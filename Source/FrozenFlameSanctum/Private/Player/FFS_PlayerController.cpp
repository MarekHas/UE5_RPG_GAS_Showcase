// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FFS_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

#include "Interfaces/MarkableInterface.h"
#include "Input/FFS_InputComponent.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "FFS_GameplayTags.h"
#include "UI/Widgets/SpatialTextWidgetComponent.h"
#include "GameFramework/Character.h"

AFFS_PlayerController::AFFS_PlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AFFS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerContext);

	AddMappingContext();
	MouseCursorSettings();
}

void AFFS_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MarkActorUnderCursor();
	AutoRun();
}

void AFFS_PlayerController::ShowDamageValue_Implementation(float DamageAmount, ACharacter* TargetCharacter,
                                                           const bool bIsHitBlocked, const bool bIsCriticalHitSuccessful)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		USpatialTextWidgetComponent* DamageText = NewObject<USpatialTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bIsHitBlocked, bIsCriticalHitSuccessful);
	}
}

void AFFS_PlayerController::AddMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (InputLocalPlayerSubsystem)
	{
		InputLocalPlayerSubsystem->AddMappingContext(PlayerContext, 0);
	}
}

void AFFS_PlayerController::MouseCursorSettings()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AFFS_PlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AFFS_PlayerController::MarkActorUnderCursor()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastMarkedActor = CurrentMarkedActor;
	CurrentMarkedActor = CursorHit.GetActor();

	if (CurrentMarkedActor != LastMarkedActor)
	{
		if (CurrentMarkedActor) 
		{
			CurrentMarkedActor->MarkActor();
		}
		if (LastMarkedActor) 
		{
			LastMarkedActor->UnmarkActor();
		} 
	}
}

void AFFS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UFFS_InputComponent* FFS_InputComponent = CastChecked<UFFS_InputComponent>(InputComponent);

	FFS_InputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFFS_PlayerController::OnMovementInputAction);
	FFS_InputComponent->BindAbilityActions(InputSettings, this, &ThisClass::InputTagPressed, &ThisClass::InputTagReleased, &ThisClass::InputTagHeld);
	FFS_InputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AFFS_PlayerController::ShiftPressed);
	FFS_InputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AFFS_PlayerController::ShiftReleased);
}



void AFFS_PlayerController::OnMovementInputAction(const FInputActionValue& InputActionValue)
{
	const FVector2D InputActionVector2D = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(Forward, InputActionVector2D.Y);
		ControlledPawn->AddMovementInput(Right, InputActionVector2D.X);
	}
}

void AFFS_PlayerController::InputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FFFS_GameplayTags::Get().Input_Left_Mouse_Button))
	{
		bTargeting = CurrentMarkedActor ? true : false;
		bAutoRunning = false;
	}
}

void AFFS_PlayerController::InputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FFFS_GameplayTags::Get().Input_Left_Mouse_Button))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				if(NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AFFS_PlayerController::InputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FFFS_GameplayTags::Get().Input_Left_Mouse_Button))
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting || bShiftKeyDown)
	{
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UFFS_AbilitySystemComponent* AFFS_PlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UFFS_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}

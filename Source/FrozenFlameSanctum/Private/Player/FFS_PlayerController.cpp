// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FFS_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/MarkableInterface.h"

AFFS_PlayerController::AFFS_PlayerController()
{
	bReplicates = true;
}

void AFFS_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MarkActorUnderCursor();
}

void AFFS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerContext);

	AddMappingContext();
	MouseCursorSettings();
}

void AFFS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFFS_PlayerController::OnMovementInputAction);
}

void AFFS_PlayerController::AddMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(InputLocalPlayerSubsystem);
	InputLocalPlayerSubsystem->AddMappingContext(PlayerContext, 0);
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

void AFFS_PlayerController::MarkActorUnderCursor()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastMarkedActor = CurrentMarkedActor;
	CurrentMarkedActor = CursorHit.GetActor();

	/*  What actor is under cursor 
	 *  1. LastActor is null && CurrentActor is null - no mark
	 *	2. LastActor is null && CurrentActor is valid - Mark Current Actor
	 *	3. LastActor is valid && CurrentActor is null - Unmark LastMarkedActor
	 *  4. Both actors are valid, and are the same actor - no mark
	 *	5. Both actors are valid, but LastActor != CurrentActor - Unmark LastActor, and Mark CurrentActor
	 */

	//Case 1
	if (LastMarkedActor == nullptr && CurrentMarkedActor == nullptr) return;
	//Case 2
	if (LastMarkedActor == nullptr && CurrentMarkedActor) { CurrentMarkedActor->MarkActor(); }
	//Case 3
	if (LastMarkedActor && CurrentMarkedActor == nullptr) { LastMarkedActor->UnmarkActor(); }
	//Case 4
	if (LastMarkedActor && CurrentMarkedActor && LastMarkedActor == CurrentMarkedActor) return;
	//Case 5
	if (LastMarkedActor && CurrentMarkedActor && LastMarkedActor != CurrentMarkedActor) 
	{
		LastMarkedActor->UnmarkActor();
		CurrentMarkedActor->MarkActor();
	}
}

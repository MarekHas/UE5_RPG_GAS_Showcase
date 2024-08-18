// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FFS_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AFFS_PlayerController::AFFS_PlayerController()
{
	bReplicates = true;
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

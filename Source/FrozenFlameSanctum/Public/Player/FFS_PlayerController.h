// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FFS_PlayerController.generated.h"

	
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFFS_PlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;
	
	void AddMappingContext();
	void MouseCursorSettings();

	void OnMovementInputAction(const FInputActionValue& InputActionValue);
};

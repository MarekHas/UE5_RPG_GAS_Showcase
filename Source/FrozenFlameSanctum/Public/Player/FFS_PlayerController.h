// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FFS_PlayerController.generated.h"

	
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IMarkableInterface;
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFFS_PlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;
	
	TScriptInterface<IMarkableInterface> LastMarkedActor;
	TScriptInterface<IMarkableInterface> CurrentMarkedActor;
	
	void AddMappingContext();
	void MouseCursorSettings();
	
	void OnMovementInputAction(const FInputActionValue& InputActionValue);

	void MarkActorUnderCursor();
};

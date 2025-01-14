// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"

#include "FFS_PlayerController.generated.h"

	
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IMarkableInterface;
class UFFS_InputSettings;
class UFFS_AbilitySystemComponent;
class USplineComponent;
class USpatialTextWidgetComponent;
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
	UFUNCTION(Client, Reliable)
	void ShowDamageValue(float DamageAmount, ACharacter* TargetCharacter, const bool bIsHitBlocked, const bool bIsCriticalHitSuccessful);
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MovementAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	TScriptInterface<IMarkableInterface> LastMarkedActor;
	TScriptInterface<IMarkableInterface> CurrentMarkedActor;
	FHitResult CursorHit;
	
	void AddMappingContext();
	void MouseCursorSettings();
	
	void OnMovementInputAction(const FInputActionValue& InputActionValue);

	void MarkActorUnderCursor();
	void InputTagPressed(FGameplayTag InputTag);
	void InputTagReleased(FGameplayTag InputTag);
	void InputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UFFS_InputSettings> InputSettings;
	UPROPERTY()
	TObjectPtr<UFFS_AbilitySystemComponent> AbilitySystemComponent;
	UFFS_AbilitySystemComponent* GetAbilitySystemComponent();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USpatialTextWidgetComponent> DamageTextComponentClass;
};

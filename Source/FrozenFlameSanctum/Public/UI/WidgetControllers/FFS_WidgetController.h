// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FFS_WidgetController.generated.h"

class UAbilitiesInfo;
class UFFS_AttributeSet;
class UFFS_AbilitySystemComponent;
class AFFS_PlayerState;
class AFFS_PlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* Controller, APlayerState* State, UAbilitySystemComponent* AbilitySystem, UAttributeSet* Attribute)
		: PlayerController(Controller), PlayerState(State), AbilitySystemComponent(AbilitySystem), AttributeSet(Attribute) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInfoFoundSignature, const FFFS_AbilityInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLevelChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_WidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAbilityInfoFoundSignature OnAbilityInfoFoundDelegate;

	void AbilityInfoBroadcast();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilitiesInfo> AbilitiesInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AFFS_PlayerController> FFS_PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AFFS_PlayerState> FFS_PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UFFS_AbilitySystemComponent> FFS_AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UFFS_AttributeSet> FFS_AttributeSet;

	AFFS_PlayerController* GetFFSPlayerController();
	AFFS_PlayerState* GetFFSPlayerState();
	UFFS_AbilitySystemComponent* GetFFSAbilitySystemComponent();
	UFFS_AttributeSet* GetFFSAttributeSet();
};

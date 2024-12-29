// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AbilitiesInfo.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "FFS_PlayerStatsWidgetController.generated.h"

class UFFS_AbilitySystemComponent;
struct FFFS_AbilityInfo;

USTRUCT(BlueprintType)
struct FNotificationWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UFFS_UserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NotificationMessage = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

class UFFS_UserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLevelChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationSignature, FNotificationWidgetRow, Notification);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FROZENFLAMESANCTUM_API UFFS_PlayerStatsWidgetController : public UFFS_WidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatsChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatsChangedSignature OnMaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatsChangedSignature OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatsChangedSignature OnMaxManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|ExperiencePoints")
	FOnPlayerStatsChangedSignature OnExperiencePointsChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerLevelChangedSignature OnPlayerLevelChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Notifications")
	FOnNotificationSignature OnNotificationDelegate;

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Notifications")
	TObjectPtr<UDataTable> NotificationWidgetsDataTable;
	
	void OnExperiencePointsChanged(int32 NewExperiencePoints);

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UFFS_PlayerStatsWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
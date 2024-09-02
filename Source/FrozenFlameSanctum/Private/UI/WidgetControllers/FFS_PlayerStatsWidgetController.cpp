// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"

void UFFS_PlayerStatsWidgetController::BroadcastInitialValues()
{
	const UFFS_AttributeSet* FFS_AttributeSet = CastChecked<UFFS_AttributeSet>(AttributeSet);

	OnHealthChangedDelegate.Broadcast(FFS_AttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(FFS_AttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(FFS_AttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(FFS_AttributeSet->GetMaxMana());
}

void UFFS_PlayerStatsWidgetController::BindCallbacksToDependencies()
{
	const UFFS_AttributeSet* FFS_AttributeSet = CastChecked<UFFS_AttributeSet>(AttributeSet);

	//Player Stats delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnHealthChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChangedDelegate.Broadcast(Data.NewValue); });
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnManaChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxManaChangedDelegate.Broadcast(Data.NewValue); });

	//Notification about effect applied delegate
	Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent)->OnEffectAppliedDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Notification"));
				if (Tag.MatchesTag(MessageTag)) 
				{
					if (NotificationWidgetsDataTable == nullptr) 
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "DT_Notficatins NULLPTR");
						return;
					}

					const FNotificationWidgetRow* Row = GetDataTableRowByTag<FNotificationWidgetRow>(NotificationWidgetsDataTable, Tag);
					if (Row == nullptr)
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "ROW NULLPTR");
						return;
					}
					OnNotificationDelegate.Broadcast(*Row);
				}
			}
		}
	);
}

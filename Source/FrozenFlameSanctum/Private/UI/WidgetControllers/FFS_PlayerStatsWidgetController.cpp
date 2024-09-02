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

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetHealthAttribute()).AddUObject(this, &UFFS_PlayerStatsWidgetController::OnHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UFFS_PlayerStatsWidgetController::OnMaxHealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetManaAttribute()).AddUObject(this, &UFFS_PlayerStatsWidgetController::OnManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxManaAttribute()).AddUObject(this, &UFFS_PlayerStatsWidgetController::OnMaxManaChanged);

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

void UFFS_PlayerStatsWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UFFS_PlayerStatsWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UFFS_PlayerStatsWidgetController::OnManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChangedDelegate.Broadcast(Data.NewValue);
}

void UFFS_PlayerStatsWidgetController::OnMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
}

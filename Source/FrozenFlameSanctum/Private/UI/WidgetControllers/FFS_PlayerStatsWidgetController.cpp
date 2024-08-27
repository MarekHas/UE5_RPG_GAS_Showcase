// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
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

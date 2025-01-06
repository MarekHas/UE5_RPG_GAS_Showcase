// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_SkillMenuWidgetController.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitiesInfo.h"
#include "Player/FFS_PlayerState.h"

void UFFS_SkillMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	AbilityInfoBroadcast();

	OnSkillPointsChangedDelegate.Broadcast(GetFFSPlayerState()->GetSkillPoints());
}

void UFFS_SkillMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetFFSAbilitySystemComponent()->OnAbilityStateChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StateTag)
	{
		if (AbilitiesInfo)
		{
			FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StateTag = StateTag;
			OnAbilityInfoFoundDelegate.Broadcast(Info);
		}
	});

	GetFFSPlayerState()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
	});
}

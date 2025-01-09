// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_SkillMenuWidgetController.h"

#include "FFS_GameplayTags.h"
#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilitiesInfo.h"
#include "Player/FFS_PlayerState.h"

void UFFS_SkillMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	AbilityInfoBroadcast();

	OnSkillPointsChangedDelegate.Broadcast(GetFFSPlayerState()->GetAttributePoints());
}

void UFFS_SkillMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetFFSAbilitySystemComponent()->OnAbilityStateChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StateTag, int32 CurrentAbilityLevel)
	{
		if (SelectedSkill.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedSkill.State = StateTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StateTag, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
			OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
		}
		
		if (AbilitiesInfo)
		{
			FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StateTag = StateTag;
			OnAbilityInfoFoundDelegate.Broadcast(Info);
		}
	});

	GetFFSPlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
		CurrentSkillPoints = SkillPoints;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedSkill.State, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
		OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	});
}

void UFFS_SkillMenuWidgetController::SkillButtonSelected(const FGameplayTag& AbilityTag)
{
	const FFFS_GameplayTags GameplayTags = FFFS_GameplayTags::Get();	
	const int32 SkillPoints = GetFFSPlayerState()->GetSkillPoints();
	FGameplayTag AbilityState;	
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Ability_Type_None);
	const FGameplayAbilitySpec* AbilitySpec = GetFFSAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityState = GameplayTags.Ability_State_Locked;
	}
	else
	{
		AbilityState = GetFFSAbilitySystemComponent()->GetAbilityStateFromSpec(*AbilitySpec);
	}
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityState, SkillPoints, bEnableSpendPoints, bEnableEquip);

	SelectedSkill.Ability = AbilityTag;
	SelectedSkill.State = AbilityState;
	
	OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void UFFS_SkillMenuWidgetController::SpendPointButtonPressed()
{
	if(GetFFSAbilitySystemComponent())
	{
		GetFFSAbilitySystemComponent()->Server_SpendSkillPoint(SelectedSkill.Ability);
	}
}

void UFFS_SkillMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints,
                                                         bool& bCanSpendSkillPoint, bool& bCanEquipWithSkill)
{
	const FFFS_GameplayTags GameplayTags = FFFS_GameplayTags::Get();
	bCanSpendSkillPoint = false;
	bCanEquipWithSkill = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_State_Available) && SkillPoints > 0)
	{
		bCanSpendSkillPoint = true;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_State_Owned))
	{
		bCanEquipWithSkill = true;
		if(SkillPoints > 0)
		{
			bCanSpendSkillPoint = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_State_Used))
	{
		if(SkillPoints > 0)
		{
			bCanSpendSkillPoint = true;
		}
	}
}

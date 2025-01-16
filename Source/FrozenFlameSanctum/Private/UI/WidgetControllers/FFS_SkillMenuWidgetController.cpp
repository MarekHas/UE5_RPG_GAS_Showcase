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
			FString Description;
			FString LevelUpAbilityDescription;
			GetFFSAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, LevelUpAbilityDescription);
			OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, LevelUpAbilityDescription);
		}
		
		if (AbilitiesInfo)
		{
			FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StateTag = StateTag;
			OnAbilityInfoFoundDelegate.Broadcast(Info);
		}
	});
	
	GetFFSAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &UFFS_SkillMenuWidgetController::OnAbilityEquipped);

	GetFFSPlayerState()->OnAttributePointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
		CurrentSkillPoints = SkillPoints;
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedSkill.State, CurrentSkillPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString LevelUpAbilityDescription;
		GetFFSAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedSkill.Ability, Description, LevelUpAbilityDescription);
		OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, LevelUpAbilityDescription);
	});

	const FGameplayTag SelectedStatus = GetFFSAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedSkill.Ability);
	if (SelectedStatus.MatchesTagExact(FFFS_GameplayTags::Get().Ability_State_Used))
	{
		SelectedSlotTag = GetFFSAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedSkill.Ability);
	}
}

void UFFS_SkillMenuWidgetController::SkillButtonSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		OnEquipEndedDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
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
	
	FString Description;
	FString LevelUpAbilityDescription;
	GetFFSAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, LevelUpAbilityDescription);
	OnSkillButtonSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, LevelUpAbilityDescription);
}

void UFFS_SkillMenuWidgetController::SpendPointButtonPressed()
{
	if(GetFFSAbilitySystemComponent())
	{
		GetFFSAbilitySystemComponent()->Server_SpendSkillPoint(SelectedSkill.Ability);
	}
}

void UFFS_SkillMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilitiesInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;
	OnEquipStartedDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag AbilityState = GetFFSAbilitySystemComponent()->GetStatusFromAbilityTag(AbilityType);
	if(AbilityState.MatchesTagExact(FFFS_GameplayTags::Get().Ability_State_Used))
	{
		SelectedSlotTag = GetFFSAbilitySystemComponent()->GetInputTagFromAbilityTag(AbilityType);
	}
}

void UFFS_SkillMenuWidgetController::DeselectSkill()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilitiesInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;
		OnEquipEndedDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedSkill.Ability = FFFS_GameplayTags::Get().Ability_Type_None;
	SelectedSkill.State = FFFS_GameplayTags::Get().Ability_State_Locked;
	
	OnSkillButtonSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void UFFS_SkillMenuWidgetController::SkillPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	
	const FGameplayTag& SelectedAbilityType = AbilitiesInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;
	GetFFSAbilitySystemComponent()->ServerEquipAbility(SelectedSkill.Ability, SlotTag);
}

void UFFS_SkillMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityState,
	const FGameplayTag& CurrentInputTag, const FGameplayTag& PreviousInputTag)
{
	bWaitingForEquipSelection = false;
	const FFFS_GameplayTags& GameplayTags = FFFS_GameplayTags::Get();
	
	FFFS_AbilityInfo PreviousAbilityInfo;
	PreviousAbilityInfo.StateTag = GameplayTags.Ability_State_Owned;
	PreviousAbilityInfo.InputTag = PreviousInputTag;
	PreviousAbilityInfo.AbilityTag = GameplayTags.Ability_Type_None;
	
	// when changing input for an ability that already had an assigned one
	OnAbilityInfoFoundDelegate.Broadcast(PreviousAbilityInfo);

	FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StateTag = AbilityState;
	Info.InputTag = CurrentInputTag;

	OnAbilityInfoFoundDelegate.Broadcast(Info);
	OnSkillReassignedDelegate.Broadcast(AbilityTag);
	OnEquipEndedDelegate.Broadcast(AbilitiesInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);

	DeselectSkill();
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
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_State_Owned)
		|| AbilityStatus.MatchesTagExact(GameplayTags.Ability_State_Used))
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

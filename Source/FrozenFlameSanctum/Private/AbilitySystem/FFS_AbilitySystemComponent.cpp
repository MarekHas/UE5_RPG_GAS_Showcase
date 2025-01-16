// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FFS_GameplayTags.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "AbilitySystem/FFS_GameplayAbility.h"
#include "AbilitySystem/Data/AbilitiesInfo.h"
#include "Interfaces/PlayerInterface.h"

//This function should be called affter InitAbilityActorInfo 
void UFFS_AbilitySystemComponent::BindToAbilitySystemDelegates()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UFFS_AbilitySystemComponent::Client_OnEffectApplied);
}

void UFFS_AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UFFS_GameplayAbility* AuraAbility = Cast<UFFS_GameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FFFS_GameplayTags::Get().Ability_State_Owned);
			
			GiveAbility(AbilitySpec);
		}
	}
	
	bStartupAbilitiesGranted = true;
	OnAbilitiesGrantedDelegate.Broadcast();
}

void UFFS_AbilitySystemComponent::AddPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UFFS_AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UFFS_AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UFFS_AbilitySystemComponent::OnAbilityGiven(const FOnAbilityGiven& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UFFS_AbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UFFS_AbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UFFS_AbilitySystemComponent::GetAbilityStateFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.State"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UFFS_AbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityStateFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UFFS_AbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UFFS_AbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

bool UFFS_AbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if(UFFS_GameplayAbility* AuraAbility = Cast<UFFS_GameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetLevelUpDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const UAbilitiesInfo* AbilityInfo = UFFS_AbilityBlueprintLibrary::GetAbilityInfo(GetAvatarActor());
	
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FFFS_GameplayTags::Get().Ability_Type_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UFFS_GameplayAbility::GetLockedDescription(
			AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequired);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UFFS_AbilitySystemComponent::UpgradeSkill(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetSkillPoints(GetAvatarActor()) > 0)
		{
			Server_UpgradeSkill(AttributeTag);
		}
	}
}

void UFFS_AbilitySystemComponent::Server_SpendSkillPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddSkillPoints(GetAvatarActor(), -1);
		}
		
		const FFFS_GameplayTags GameplayTags = FFFS_GameplayTags::Get();
		FGameplayTag Status = GetAbilityStateFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Ability_State_Available))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Ability_State_Available);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_State_Owned);
			Status = GameplayTags.Ability_State_Owned;
		}
		else if (Status.MatchesTagExact(GameplayTags.Ability_State_Used) || Status.MatchesTagExact(GameplayTags.Ability_State_Owned))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityState(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UFFS_AbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FFFS_GameplayTags& GameplayTags = FFFS_GameplayTags::Get();
		const FGameplayTag& PreviousSlotInputTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& AbilityStateTag = GetAbilityStateFromSpec(*AbilitySpec);
		const bool bStatusValid = AbilityStateTag == GameplayTags.Ability_State_Used || AbilityStateTag == GameplayTags.Ability_State_Owned;
		if (bStatusValid)
		{
			// Remove InputTag from ability that currently using this tag
			ClearInputTag(Slot);
			//Remove tag from ability
			RemoveInputTag(AbilitySpec);
			
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);
			if (AbilityStateTag.MatchesTagExact(GameplayTags.Ability_State_Owned))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Ability_State_Owned);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_State_Used);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Ability_State_Used, Slot, PreviousSlotInputTag);
	}
}

void UFFS_AbilitySystemComponent::ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& State,
	const FGameplayTag& CurrentInputTag, const FGameplayTag& PreviousInputTag)
{
	AbilityEquipped.Broadcast(AbilityTag, State, CurrentInputTag, PreviousInputTag);
}

void UFFS_AbilitySystemComponent::UpdateAbilityState(int32 Level)
{
	UAbilitiesInfo* AbilityInfo = UFFS_AbilityBlueprintLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FFFS_AbilityInfo& Info : AbilityInfo->AbilitiesData)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequired) continue;
		
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FFFS_GameplayTags::Get().Ability_State_Available);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);

			ClientUpdateAbilityState(Info.AbilityTag, FFFS_GameplayTags::Get().Ability_State_Available,1);
		}
	}
}

void UFFS_AbilitySystemComponent::RemoveInputTag(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void UFFS_AbilitySystemComponent::ClearInputTag(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			RemoveInputTag(&Spec);
		}
	}
}

bool UFFS_AbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UFFS_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilitiesGranted)
	{
		bStartupAbilitiesGranted = true;
		OnAbilitiesGrantedDelegate.Broadcast();
	}
}

void UFFS_AbilitySystemComponent::ClientUpdateAbilityState_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StateTag, int32 AbilityLevel)
{
	OnAbilityStateChangedDelegate.Broadcast(AbilityTag, StateTag, AbilityLevel);
}

void UFFS_AbilitySystemComponent::Server_UpgradeSkill_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddSkillPoints(GetAvatarActor(),-1);
	}
}

void UFFS_AbilitySystemComponent::Client_OnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAppliedDelegate.Broadcast(TagContainer);
}

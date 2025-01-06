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

			ClientUpdateAbilityState(Info.AbilityTag,FFFS_GameplayTags::Get().Ability_State_Available);
		}
	}
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
	const FGameplayTag& StateTag)
{
	OnAbilityStateChangedDelegate.Broadcast(AbilityTag, StateTag);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FFS_PassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FFS_AbilitySystemComponent.h"

void UFFS_PassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UFFS_AbilitySystemComponent* FFS_AbilitySystemComponent = Cast<UFFS_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		FFS_AbilitySystemComponent->OnDeactivatePassiveAbilityDelegate.AddUObject(this, &UFFS_PassiveAbility::ReceiveDeactivate);
	}
}

void UFFS_PassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

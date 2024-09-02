// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilitySystemComponent.h"

//This function should be called affter InitAbilityActorInfo 
void UFFS_AbilitySystemComponent::BindToAbilitySystemDelegates()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UFFS_AbilitySystemComponent::OnEffectApplied);
}

void UFFS_AbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	if (TagContainer.IsEmpty()) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, "What");
	}
	OnEffectAppliedDelegate.Broadcast(TagContainer);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "FFS_AbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAppliedSignature, const FGameplayTagContainer&);
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//This function should be called affter InitAbilityActorInfo 
	void BindToAbilitySystemDelegates();
	FOnEffectAppliedSignature OnEffectAppliedDelegate;
protected:
	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};

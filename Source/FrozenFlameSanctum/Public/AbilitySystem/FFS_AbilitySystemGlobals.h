// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FFS_AbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};

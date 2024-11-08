// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilitySystemGlobals.h"

#include "FFS_AbilityTypes.h"

FGameplayEffectContext* UFFS_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFFS_GameplayEffectContext();
}

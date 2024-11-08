// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/FFS_GameplayAbility.h"
#include "FFS_DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_DamageGameplayAbility : public UFFS_GameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypes;
 };

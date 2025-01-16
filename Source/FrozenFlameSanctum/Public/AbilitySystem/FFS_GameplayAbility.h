// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FFS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetLevelUpDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};

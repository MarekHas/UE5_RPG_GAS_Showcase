// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FFS_ProjectileSpell.h"
#include "FireBolt_ProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFireBolt_ProjectileSpell : public UFFS_ProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetLevelUpDescription(int32 Level) override;
};

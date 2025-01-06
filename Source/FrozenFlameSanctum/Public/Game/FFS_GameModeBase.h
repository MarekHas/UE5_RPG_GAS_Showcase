// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FFS_GameModeBase.generated.h"

class UAbilitiesInfo;
class UEnemiesData;

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy data")
	TObjectPtr<UEnemiesData> EnemiesData;
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilitiesInfo> AbilityInfo;
};

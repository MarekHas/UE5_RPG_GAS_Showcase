// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterProgressData.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpData
{
	GENERATED_BODY()
	//Experience points required to level up
	UPROPERTY(EditDefaultsOnly)
	int32  ExperiencePointsRequired= 0;
	//Points received after level up
	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointsReceived = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointsReceived = 1;
};
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UCharacterProgressData : public UDataAsset
{
	GENERATED_BODY()

public:
UPROPERTY(EditDefaultsOnly)
	TArray<FLevelUpData> LevelUpData;

	int32 GetCharacterLevel(int32 InExperiencePoints) const;
	
};

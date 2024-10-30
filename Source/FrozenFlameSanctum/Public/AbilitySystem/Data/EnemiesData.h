// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemiesData.generated.h"

class UGameplayEffect;
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee,
	Range,
	Magic
};

USTRUCT(BlueprintType)
struct FEnemyDefaultStats
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Default Stats")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UEnemiesData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TMap<EEnemyType, FEnemyDefaultStats> EnemyInfo;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	FEnemyDefaultStats GetClassDefaultInfo(EEnemyType EnemyType);
};

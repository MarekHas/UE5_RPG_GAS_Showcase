// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "EnemiesData.generated.h"

class UGameplayEffect;
class UGameplayAbility;

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
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	FScalableFloat ExperiencePointsReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UEnemiesData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TMap<EEnemyType, FEnemyDefaultStats> EnemyInfo;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TSubclassOf<UGameplayEffect> VitalAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyData|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
	
	FEnemyDefaultStats GetClassDefaultInfo(EEnemyType EnemyType);
};

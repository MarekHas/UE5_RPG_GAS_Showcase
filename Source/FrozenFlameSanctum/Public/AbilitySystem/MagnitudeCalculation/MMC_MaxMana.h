// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseStatMultiplier = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterLevelMultiplier = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue = 50.f;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;
};

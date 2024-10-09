// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BaseStatMultiplier = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CharacterLevelMultiplier = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue = 100.f;

private:
	FGameplayEffectAttributeCaptureDefinition VitalityDefinition;
};

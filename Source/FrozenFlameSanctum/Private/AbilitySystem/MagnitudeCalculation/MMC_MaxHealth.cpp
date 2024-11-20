// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MagnitudeCalculation/MMC_MaxHealth.h"

#include "AbilitySystem/FFS_AttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDefinition.AttributeToCapture = UFFS_AttributeSet::GetVitalityAttribute();

	VitalityDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Vitality = 0.f;
	GetCapturedAttributeMagnitude(VitalityDefinition, Spec, EvaluationParameters, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);
	
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return BaseValue + BaseStatMultiplier * Vitality + CharacterLevelMultiplier * PlayerLevel;
}

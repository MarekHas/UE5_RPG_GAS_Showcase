// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecutionCalculation/Damage_ExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "FFS_GameplayTags.h"
#include "FFS_AbilityTypes.h"

#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "AbilitySystem/Data/EnemiesData.h"

struct FDamageStats
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> GameplayTagToGameplayEffectDefinition;
	
	FDamageStats()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, BlockChance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFFS_AttributeSet, PhysicalResistance, Target, false);

		const FFFS_GameplayTags& Tags = FFFS_GameplayTags::Get();
		
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Resistance_Lightning, LightningResistanceDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Resistance_Magic, MagicResistanceDef);
		GameplayTagToGameplayEffectDefinition.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const FDamageStats& DamageStatsData()
{
	static FDamageStats DamageStatsData;
	return DamageStatsData;
}

UDamage_ExecutionCalculation::UDamage_ExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatsData().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatsData().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatsData().BlockChanceDef);

	RelevantAttributesToCapture.Add(DamageStatsData().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatsData().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatsData().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatsData().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatsData().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatsData().MagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatsData().PhysicalResistanceDef);
}

void UDamage_ExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAbilitySystem = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystem = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceAbilitySystem ? SourceAbilitySystem->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAbilitySystem ? TargetAbilitySystem->GetAvatarActor() : nullptr;
	
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FFFS_GameplayTags::Get().DamageTypeToResistanceType)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		auto T = FDamageStats().GameplayTagToGameplayEffectDefinition;
		checkf(FDamageStats().GameplayTagToGameplayEffectDefinition.Contains(ResistanceTag), TEXT("GameplayTagToGameplayEffectDefinition doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition EffectAttributeCaptureDefinition = FDamageStats().GameplayTagToGameplayEffectDefinition[ResistanceTag];
		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(EffectAttributeCaptureDefinition, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;
		Damage += DamageTypeValue;
	}

	//Reduce damage when block is successful
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	UFFS_AbilityBlueprintLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	//When block is successful cut damage by half
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UEnemiesData* CharacterClassInfo = UFFS_AbilityBlueprintLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationMultiplier = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationMultiplier) / 100.f;
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorMultiplier = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	Damage *= (100 - EffectiveArmor * EffectiveArmorMultiplier) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	//Critical hit damage
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatsData().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceMultiplier = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);
	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceMultiplier;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	UFFS_AbilityBlueprintLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);
	// Double damage plus a bonus if critical hit
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UFFS_AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

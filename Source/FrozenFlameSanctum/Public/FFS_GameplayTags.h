// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Singleton containing native Gameplay Tags
 */
struct FFFS_GameplayTags
{
public:
	static const FFFS_GameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resistance;
	FGameplayTag Attribute_Primary_Vitality;
	FGameplayTag Attribute_Primary_Dexterity;

	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;

	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;

	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Magic;
	FGameplayTag Attribute_Resistance_Physical;

	FGameplayTag Attribute_GameEvent_ExperienceReceived;
	
	FGameplayTag Input_Left_Mouse_Button;
	FGameplayTag Input_Right_Mouse_Button;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;
	FGameplayTag Input_Passive_1;
	FGameplayTag Input_Passive_2;
	
	FGameplayTag HitReact;
	FGameplayTag Damage;
	//Damage Types
	FGameplayTag FireDamage;
	FGameplayTag LightningDamage;
	FGameplayTag MagicalDamage;
	FGameplayTag PhysicalDamage;

	FGameplayTag Ability_Attack;
	
	FGameplayTag Ability_Fire_FireBolt;

	FGameplayTag Cooldown_Fire_FireBolt;
	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistanceType;
protected:
private:
	static FFFS_GameplayTags GameplayTags;
};
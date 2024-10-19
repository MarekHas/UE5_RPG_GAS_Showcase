// Fill out your copyright notice in the Description page of Project Settings.
#include "FFS_GameplayTags.h"
#include "GameplayTagsManager.h"

FFFS_GameplayTags FFFS_GameplayTags::GameplayTags;

void FFFS_GameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Strength"),
		FString("Increases physical skills")
	);
	GameplayTags.Attribute_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Dexterity"),
		FString("Increases combat stats like armor penetration or chance for critical damage")
	);
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"),
		FString("Increases magical skills")
	);
	GameplayTags.Attribute_Primary_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Resistance"),
		FString("Increases resistance to damage")
	);
	GameplayTags.Attribute_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Primary.Vitality"),
		FString("Base for health")
	);
	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"),
		FString("Maximum value of Health")
	);
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"),
		FString("Maximum value of Mana")
	);
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"),
		FString("Health regenerated every 1 second")
	);
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"),
		FString("Mana regenerated every 1 second")
	);

	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.Armor"),
		FString("reduce damage, increace chance to block")
	);
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ArmorPenetration"),
		FString("increases Critical Hit Chance and chance to ignore enemy armor")
	);
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.BlockChance"),
		FString("Chance to reduce damge")
	);
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitChance"),
		FString("Chance to add critical damge")
	);
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitDamage"),
		FString("Damge points for critical damge")
	);
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitResistance"),
		FString("Reduce chance to get critical damage")
	);
}
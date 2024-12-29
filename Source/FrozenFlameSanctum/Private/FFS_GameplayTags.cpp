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

	/*
 * Input Tags
 */

	GameplayTags.Input_Left_Mouse_Button = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LeftMouseButton"),
		FString("Input Tag for Left Mouse Button")
	);
	GameplayTags.Input_Right_Mouse_Button = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RightMouseButton"),
		FString("Input Tag for Right Mouse Button")
	);
	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"),
		FString("Input Tag for 1 key")
	);
	GameplayTags.Input_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"),
		FString("Input Tag for 2 key")
	);
	GameplayTags.Input_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"),
		FString("Input Tag for 3 key")
	);
	GameplayTags.Input_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"),
		FString("Input Tag for 4 key")
	);
	GameplayTags.Input_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Input.Passive.1"),
			FString("Input Tag Passive Ability 1")
			);
	GameplayTags.Input_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Passive.2"),
		FString("Input Tag Passive Ability 2")
		);
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);
	
	//Damage Types
	GameplayTags.FireDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	GameplayTags.LightningDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Lightning"),
			FString("Lightning Damage Type")
			);
	GameplayTags.MagicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Magical"),
		FString("Magical Damage Type")
		);
	GameplayTags.PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);

	//Resistance Types
	GameplayTags.Attribute_Resistance_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Attribute.Resistance.Magic"),
			FString("Resistance to magic damage")
			);
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Fire"),
		FString("Resistance to Fire damage")
		);
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
		);
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Resistance.Physical"),
		FString("Resistance to Physical damage")
		);

	//Game Event Attributes
	GameplayTags.Attribute_GameEvent_ExperienceReceived = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attribute.GameEvent.ExperienceReceived"),
	FString("Received experience points for killing enemy")
	);

	//Map Damage types to resistance
	GameplayTags.DamageTypeToResistanceType.Add(GameplayTags.PhysicalDamage, GameplayTags.Attribute_Resistance_Physical);
	GameplayTags.DamageTypeToResistanceType.Add(GameplayTags.FireDamage, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypeToResistanceType.Add(GameplayTags.LightningDamage, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypeToResistanceType.Add(GameplayTags.MagicalDamage, GameplayTags.Attribute_Resistance_Magic);

	/*
	 * Abilities
	 */
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Attack"),
		FString("Attack Ability Tag")
		);

	GameplayTags.Ability_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
		);
	
	GameplayTags.HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("HitReact"),
		FString("React to being hit")
	);

	/*
	* Cooldown
	*/
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("Cooldown Tag for FireBolt ability")
		);
}
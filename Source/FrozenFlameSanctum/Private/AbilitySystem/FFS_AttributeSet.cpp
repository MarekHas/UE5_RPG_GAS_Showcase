// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "FFS_GameplayTags.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Player/FFS_PlayerController.h"

UFFS_AttributeSet::UFFS_AttributeSet()
{
	const FFFS_GameplayTags& GameplayTags = FFFS_GameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Resistance, GetResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Vitality, GetVitalityAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Magic, GetMagicResistanceAttribute);
}

void UFFS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Vitality, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFFS_AttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UFFS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Health, OldHealth);
}

void UFFS_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Mana, OldMana);
}

void UFFS_AttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Strength, OldStrength);
}

void UFFS_AttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Dexterity, OldDexterity);
}

void UFFS_AttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Intelligence, OldIntelligence);
}

void UFFS_AttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Resistance, OldResistance);
}

void UFFS_AttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Vitality, OldVitality);
}

void UFFS_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, Armor, OldArmor);
}

void UFFS_AttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UFFS_AttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, BlockChance, OldBlockChance);
}

void UFFS_AttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UFFS_AttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UFFS_AttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UFFS_AttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UFFS_AttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UFFS_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, MaxMana, OldMaxMana);
}

void UFFS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, MaxHealth, OldMaxHealth);
}

void UFFS_AttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UFFS_AttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, FireResistance, OldFireResistance);
}

void UFFS_AttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, LightningResistance, OldLightningResistance);
}

void UFFS_AttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFFS_AttributeSet, MagicResistance, OldMagicResistance);
}

void UFFS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UFFS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
	//Health and Mana
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	//Incoming Damge
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			if (const bool bIsDead = NewHealth <= 0.f)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Death();
				}
				SendExperiencePointsReceivedEvent(EffectProperties);
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FFFS_GameplayTags::Get().HitReact);
				EffectProperties.TargetAbilitySystem->TryActivateAbilitiesByTag(TagContainer);
			}
			
			const bool bBlockedHit = UFFS_AbilityBlueprintLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			const bool bCriticalHit = UFFS_AbilityBlueprintLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);

			ShowDamageText(EffectProperties, LocalIncomingDamage, bBlockedHit, bCriticalHit);
		}
	}
	
	//Received Experience
	if (Data.EvaluatedData.Attribute == GetExperiencePointsReceivedAttribute())
	{
		const float LocalIncomingXP = GetExperiencePointsReceived();
		SetExperiencePointsReceived(50.f);
		
		if (EffectProperties.SourceCharacter->Implements<UPlayerInterface>()
			&& EffectProperties.SourceCharacter->Implements<UCombatInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.SourceCharacter);
			const int32 CurrentExperience = IPlayerInterface::Execute_GetExperiencePoints(EffectProperties.SourceCharacter);
			const int32 NewLevel = IPlayerInterface::Execute_CheckLevelForGivenExperience(EffectProperties.SourceCharacter, CurrentExperience + LocalIncomingXP);
			const int32 NumLevelUps = NewLevel - CurrentLevel;

			if (NumLevelUps > 0)
			{
				const int32 SkillPointsReceived = IPlayerInterface::Execute_GetSkillPointsReceived(EffectProperties.SourceCharacter, CurrentLevel);
				const int32 SpellPointsReceived = IPlayerInterface::Execute_GetSpellPointsReceived(EffectProperties.SourceCharacter, CurrentLevel);
				
				IPlayerInterface::Execute_AddPlayerLevel(EffectProperties.SourceCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddSkillPoints(EffectProperties.SourceCharacter, SkillPointsReceived);
				IPlayerInterface::Execute_AddSpellPoints(EffectProperties.SourceCharacter, SpellPointsReceived);
	
				SetHealth(GetMaxHealth());
				SetMana(GetMaxMana());
				
				IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
			}
			
			IPlayerInterface::Execute_AddExperiencePoints(EffectProperties.SourceCharacter, 50);
		}
	}
}

void UFFS_AttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const
{
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceAbilitySystem = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProperties.SourceAbilitySystem) && EffectProperties.SourceAbilitySystem->AbilityActorInfo.IsValid() && EffectProperties.SourceAbilitySystem->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceAbilitySystem->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceController = EffectProperties.SourceAbilitySystem->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);
	}
}

void UFFS_AttributeSet::ShowDamageText(const FEffectProperties& EffectProperties, const float Damage, const bool bBlockedHit, const bool bCriticalHit) const
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (AFFS_PlayerController* PlayerController = Cast<AFFS_PlayerController>(EffectProperties.SourceCharacter->Controller))
		{
			PlayerController->ShowDamageValue(Damage, EffectProperties.TargetCharacter, bBlockedHit,bCriticalHit);
			return;
		}
		if (AFFS_PlayerController* PlayerController = Cast<AFFS_PlayerController>(EffectProperties.TargetCharacter->Controller))
		{
			PlayerController->ShowDamageValue(Damage, EffectProperties.TargetCharacter, bBlockedHit,bCriticalHit);
		}
	}
}

void UFFS_AttributeSet::SendExperiencePointsReceivedEvent(const FEffectProperties& EffectProperties)
{
	if (EffectProperties.TargetCharacter->Implements<UEnemyInterface>()
		&& EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel =  ICombatInterface::Execute_GetPlayerLevel(EffectProperties.TargetCharacter);
		const EEnemyType TargetClass = IEnemyInterface::Execute_GetEnemyType(EffectProperties.TargetCharacter);
		const int32 XPReward = UFFS_AbilityBlueprintLibrary::ExperiencePointsForKilledEnemy(EffectProperties.TargetCharacter, TargetClass, TargetLevel);
		const FFFS_GameplayTags& GameplayTags = FFFS_GameplayTags::Get();
		FGameplayEventData Payload;
		
		Payload.Instigator = EffectProperties.SourceCharacter;
		Payload.EventTag = GameplayTags.Attribute_GameEvent_ExperienceReceived;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, GameplayTags.Attribute_GameEvent_ExperienceReceived, Payload);
	}
}

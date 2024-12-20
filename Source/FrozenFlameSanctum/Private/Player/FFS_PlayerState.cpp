// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FFS_PlayerState.h"

#include "Net/UnrealNetwork.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"

AFFS_PlayerState::AFFS_PlayerState()
{
	NetUpdateFrequency = 100.f;

	SetupAbilitySystemComponent();

	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");
}

void AFFS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFFS_PlayerState, PlayerLevel);
	DOREPLIFETIME(AFFS_PlayerState, ExperiencePoints);
	DOREPLIFETIME(AFFS_PlayerState, SkillPoints);
	DOREPLIFETIME(AFFS_PlayerState, SpellPoints);
}

void AFFS_PlayerState::OnRep_Level(int32 OldLevel)
{
	OnPlayerLevelChangedDelegate.Broadcast(OldLevel);
}

void AFFS_PlayerState::OnRep_ExperiencePoints(int32 OldExperiencePoints)
{
	OnExperiencePointsChangedDelegate.Broadcast(OldExperiencePoints);
}

void AFFS_PlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AFFS_PlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AFFS_PlayerState::SetupAbilitySystemComponent()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFFS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

//~ Begin IAbilitySystemInterface Interface
UAbilitySystemComponent* AFFS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
//~ End IAbilitySystemInterface Interface

void AFFS_PlayerState::AddExperiencePoints(const int32 InPoints)
{
	ExperiencePoints += InPoints;
	OnExperiencePointsChangedDelegate.Broadcast(ExperiencePoints);
}

void AFFS_PlayerState::SetExperiencePoints(const int32 InPoints)
{
	ExperiencePoints = InPoints;
	OnExperiencePointsChangedDelegate.Broadcast(ExperiencePoints);
}

void AFFS_PlayerState::AddSkillPoints(int32 InPoints)
{
	SkillPoints += InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AFFS_PlayerState::AddSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AFFS_PlayerState::LevelUp(const int32 InLevel)
{
	PlayerLevel += InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

void AFFS_PlayerState::SetLevel(const int32 InLevel)
{
	PlayerLevel = InLevel;
	OnPlayerLevelChangedDelegate.Broadcast(PlayerLevel);
}

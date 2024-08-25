// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FFS_PlayerState.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"

AFFS_PlayerState::AFFS_PlayerState()
{
	NetUpdateFrequency = 100.f;

	SetupAbilitySystemComponent();

	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");
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
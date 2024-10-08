// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/FFS_EnemyCharacter.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"

AFFS_EnemyCharacter::AFFS_EnemyCharacter()
{
	//This settings is needed for marking actor, in player controller is checked if cursor hit actor on visiblity chanel with block 
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SetupAbilitySystemComponent();

	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");
}

void AFFS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}

void AFFS_EnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent)->BindToAbilitySystemDelegates();
}

void AFFS_EnemyCharacter::SetupAbilitySystemComponent()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFFS_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AFFS_EnemyCharacter::MarkActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
}

void AFFS_EnemyCharacter::UnmarkActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
}

int32 AFFS_EnemyCharacter::GetPlayerLevel()
{
	return Level;
}



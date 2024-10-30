// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/FFS_EnemyCharacter.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/FFS_UserWidget.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"

AFFS_EnemyCharacter::AFFS_EnemyCharacter()
{
	//This settings is needed for marking actor, in player controller is checked if cursor hit actor on visiblity chanel with block 
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SetupAbilitySystemComponent();

	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AFFS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	if (UFFS_UserWidget* FFS_UserWidget = Cast<UFFS_UserWidget>(HealthBar->GetUserWidgetObject()))
	{
		FFS_UserWidget->SetWidgetController(this);
	}

	if (const UFFS_AttributeSet* FFS_AttributeSet = Cast<UFFS_AttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FFS_AttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FFS_AttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		OnHealthChanged.Broadcast(FFS_AttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(FFS_AttributeSet->GetMaxHealth());
	}
}

void AFFS_EnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent)->BindToAbilitySystemDelegates();

	InitDefaultStats();
}

void AFFS_EnemyCharacter::InitDefaultStats() const
{
	UFFS_AbilityBlueprintLibrary::InitializeDefaultAttributes(this, EnemyType, Level, AbilitySystemComponent);
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



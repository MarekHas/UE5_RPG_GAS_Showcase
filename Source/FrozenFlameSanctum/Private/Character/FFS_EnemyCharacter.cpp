// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/FFS_EnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "UI/Widgets/FFS_UserWidget.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "FFS_GameplayTags.h"

AFFS_EnemyCharacter::AFFS_EnemyCharacter()
{
	//This settings is needed for marking actor, in player controller is checked if cursor hit actor on visiblity chanel with block 
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SetupAbilitySystemComponent();

	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AFFS_EnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AFFS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	check(AbilitySystemComponent);
	InitAbilityActorInfo();

	if (HasAuthority()) 
	{
		UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	
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
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FFFS_GameplayTags::Get().HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AFFS_EnemyCharacter::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(FFS_AttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(FFS_AttributeSet->GetMaxHealth());
	}
}

void AFFS_EnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent)->BindToAbilitySystemDelegates();
	if(HasAuthority())
	{
		InitDefaultStats();
	}
}

void AFFS_EnemyCharacter::InitDefaultStats() const
{
	UFFS_AbilityBlueprintLibrary::InitializeDefaultAttributes(this, EnemyType, Level, AbilitySystemComponent);
	UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
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

void AFFS_EnemyCharacter::Death()
{
	SetLifeSpan(LifeTime);
	Super::Death();
}



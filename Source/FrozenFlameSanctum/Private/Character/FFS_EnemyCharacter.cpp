// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/FFS_EnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "UI/Widgets/FFS_UserWidget.h"
#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "FFS_GameplayTags.h"
#include "AI/FFS_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AFFS_EnemyCharacter::AFFS_EnemyCharacter()
{
	//This settings is needed for marking actor, in player controller is checked if cursor hit actor on visiblity chanel with block 
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	SetupAbilitySystemComponent();
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UFFS_AttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AFFS_EnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FFS_AIController = Cast<AFFS_AIController>(NewController);
	FFS_AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	FFS_AIController->RunBehaviorTree(BehaviorTree);
	
	FFS_AIController->GetBlackboardComponent()->SetValueAsBool(FName("TakeHit"), false);
	FFS_AIController->GetBlackboardComponent()->SetValueAsBool(FName("RangeAttack"), EnemyType == EEnemyType::Range);
}

void AFFS_EnemyCharacter::SetAttackTarget_Implementation(AActor* InTarget)
{
	AttackTarget = InTarget;
}

AActor* AFFS_EnemyCharacter::GetAttackTarget_Implementation() const
{
	return AttackTarget;
}

EEnemyType AFFS_EnemyCharacter::GetEnemyType_Implementation()
{
	return EnemyType;
}

void AFFS_EnemyCharacter::HitReactionTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if(FFS_AIController && FFS_AIController->GetBlackboardComponent())
	{
		FFS_AIController->GetBlackboardComponent()->SetValueAsBool(FName("TakeHit"), bHitReacting);
	}
}

void AFFS_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	check(AbilitySystemComponent);
	InitAbilityActorInfo();

	// if (HasAuthority()) 
	// {
	// 	UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent, EnemyType);
	// }
	
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
			&AFFS_EnemyCharacter::HitReactionTagChanged
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
	UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent,EnemyType);
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

int32 AFFS_EnemyCharacter::GetPlayerLevel_Implementation()
{
	return Level;
}

void AFFS_EnemyCharacter::Death()
{
	SetLifeSpan(LifeTime);
	if(FFS_AIController) FFS_AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	Super::Death();
}



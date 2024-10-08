// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_CharacterBase.h"
#include "AbilitySystemComponent.h"

// Sets default values
AFFS_CharacterBase::AFFS_CharacterBase()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AFFS_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AFFS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFFS_CharacterBase::InitAbilityActorInfo()
{
}

void AFFS_CharacterBase::InitDefaultStats()
{
	InitStatsFromEffect(InitialCharacterStats, 1.f);
	InitStatsFromEffect(InitialDerivedStats, 1.f);
}

void AFFS_CharacterBase::InitStatsFromEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FFS_CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "FrozenFlameSanctum/FrozenFlameSanctum.h"

// Sets default values
AFFS_CharacterBase::AFFS_CharacterBase()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_ProjectileSpell, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AFFS_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AFFS_CharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AFFS_CharacterBase::Death()
{
	//Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Multicast_OnDeath();
}

void AFFS_CharacterBase::Multicast_OnDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AFFS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AFFS_CharacterBase::GetCombatSocketLocation()
{
	check(GetMesh());
	return GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

void AFFS_CharacterBase::InitAbilityActorInfo()
{
}

void AFFS_CharacterBase::InitDefaultStats() const
{
	InitStatsFromEffect(InitialBaseStats, 1.f);
	InitStatsFromEffect(InitialDerivedStats, 1.f);
	//This should be init after all other atributes are initialized
	InitStatsFromEffect(InitialLiveStats, 1.f);
}

void AFFS_CharacterBase::InitStatsFromEffect(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AFFS_CharacterBase::AddCharacterAbilities()
{
	UFFS_AbilitySystemComponent* FFS_AbilitySystemComponent = CastChecked<UFFS_AbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	FFS_AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

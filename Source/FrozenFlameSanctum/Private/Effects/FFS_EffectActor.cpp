// Fill out your copyright notice in the Description page of Project Settings.

#include "Effects/FFS_EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AFFS_EffectActor::AFFS_EffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AFFS_EffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFFS_EffectActor::ApplyEffectToActor(TSubclassOf<UGameplayEffect> GameplayEffectClass, AActor* AffectedActor)
{
	UAbilitySystemComponent* AffectedActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AffectedActor);
	if (AffectedActorAbilitySystemComponent == nullptr) return;

	check(GameplayEffectClass);

	//Wrapper for gameplay effect context
	FGameplayEffectContextHandle EffectContextHandle = AffectedActorAbilitySystemComponent->MakeEffectContext();
	//What causing the effect
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AffectedActorAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	AffectedActorAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

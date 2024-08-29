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

void AFFS_EffectActor::ApplyEffectToActor(TSubclassOf<UGameplayEffect> InGameplayEffectClass, AActor* AffectedActor)
{
	UAbilitySystemComponent* AffectedActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AffectedActor);
	if (AffectedActorAbilitySystemComponent == nullptr) return;

	check(InGameplayEffectClass);

	//Wrapper for gameplay effect context
	FGameplayEffectContextHandle EffectContextHandle = AffectedActorAbilitySystemComponent->MakeEffectContext();
	//What causing the effect
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = AffectedActorAbilitySystemComponent->MakeOutgoingSpec(InGameplayEffectClass, EffectLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = AffectedActorAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	const bool bIsInfiniteEffect = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfiniteEffect && RemovalType == EEffectRemovalType::OnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, AffectedActorAbilitySystemComponent);
	}
}

void AFFS_EffectActor::OnOverlap(AActor* OverlapActor)
{
	if (ApplicationType == EEffectApplicationType::OnOverlap) 
	{
		ApplyEffectToActor(GameplayEffectClass, OverlapActor);
	}
}

void AFFS_EffectActor::OnEndOverlap(AActor* OverlapActor)
{
	if (ApplicationType == EEffectApplicationType::OnEndOverlap)
	{
		ApplyEffectToActor(GameplayEffectClass, OverlapActor);
	}

	if (RemovalType == EEffectRemovalType::OnEndOverlap)
	{
		UAbilitySystemComponent* OverlapActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapActor);
		if (!IsValid(OverlapActorAbilitySystemComponent)) return;

		TArray<FActiveGameplayEffectHandle> EffectsToRemove;
		
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffect : ActiveEffectHandles)
		{
			if (OverlapActorAbilitySystemComponent == ActiveEffect.Value)
			{
				OverlapActorAbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffect.Key, 1);
				EffectsToRemove.Add(ActiveEffect.Key);
			}
		}

		for (FActiveGameplayEffectHandle& Effect : EffectsToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Effect);
		}
	}
}

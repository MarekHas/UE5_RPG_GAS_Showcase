// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FFS_ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"

#include "Effects/FFS_Projectile.h"
#include "FFS_GameplayTags.h"

void UFFS_ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
}

void UFFS_ProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if(GetAvatarActorFromActorInfo()->HasAuthority()) 
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
		if (CombatInterface)
		{
			const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
			
			FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
			Rotation.Pitch = 0.f;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotation.Quaternion());

			AFFS_Projectile* Projectile = GetWorld()->SpawnActorDeferred<AFFS_Projectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			
			const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			const FGameplayEffectSpecHandle SpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext());
			
			const FFFS_GameplayTags GameplayTags = FFFS_GameplayTags::Get();
			const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
			
			Projectile->DamageEffectSpecHandle = SpecHandle;

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
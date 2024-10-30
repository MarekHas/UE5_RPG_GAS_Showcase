// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/FFS_GameplayAbility.h"
#include "FFS_ProjectileSpell.generated.h"

class AFFS_Projectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_ProjectileSpell : public UFFS_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AFFS_Projectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
};

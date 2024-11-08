// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/EnemiesData.h"

#include "FFS_AbilityBlueprintLibrary.generated.h"

class UFFS_PlayerStatsWidgetController;
class UFFS_AttributesWidgetController;
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AbilityBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|WidgetController")
	static UFFS_PlayerStatsWidgetController* GetWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|WidgetController")
	static UFFS_AttributesWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|EnemyType")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEnemyType EnemyType, 
		float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static UEnemiesData* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit);
};

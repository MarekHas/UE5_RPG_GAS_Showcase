// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/EnemiesData.h"

#include "FFS_AbilityBlueprintLibrary.generated.h"

struct FWidgetControllerParams;
class UAbilitiesInfo;
class UFFS_SkillMenuWidgetController;
class AFFS_GameHUD;
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
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,
		FWidgetControllerParams& OutWidgetControllerParams,AFFS_GameHUD*& OutGameHUD);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_PlayerStatsWidgetController* GetWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_AttributesWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_SkillMenuWidgetController* GetSkillMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|EnemyType")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEnemyType EnemyType, 
		float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, EEnemyType EnemyType);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static UEnemiesData* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilitiesInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersInRange(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore, float Range, const FVector& SphereOrigin);
	
	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsCriticalHit);

	static int32 ExperiencePointsForKilledEnemy(const UObject* WorldContextObject, EEnemyType EnemyType, int32 Level);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/EnemiesData.h"

#include "FFS_AbilityBlueprintLibrary.generated.h"

struct FWidgetControllerParams;
class UAbilitiesInfo;
class UFFS_AbilitySystemComponent;
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
	static bool NativeDoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,bool& OutConfirmType);
	static UFFS_AbilitySystemComponent* NativeGetFFS_AbilitySystemFromActor(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor,FGameplayTag TagToRemove);
	UFUNCTION(BlueprintPure, Category="FFS_AbilityBlueprintLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,
		FWidgetControllerParams& OutWidgetControllerParams,AFFS_GameHUD*& OutGameHUD);
	
	UFUNCTION(BlueprintPure, Category="FFS_AbilityBlueprintLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_PlayerStatsWidgetController* GetWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="FFS_AbilityBlueprintLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_AttributesWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category="FFS_AbilityBlueprintLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UFFS_SkillMenuWidgetController* GetSkillMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|EnemyType")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEnemyType EnemyType, 
		float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, EEnemyType EnemyType);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static UEnemiesData* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category="FFS_AbilityBlueprintLibrary|CharacterClassDefaults")
	static UAbilitiesInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "FFS_AbilityBlueprintLibrary|GameplayMechanics")
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

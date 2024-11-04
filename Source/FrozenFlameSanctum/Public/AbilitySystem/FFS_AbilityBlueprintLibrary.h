// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintPure, Category = "AbilityBlueprintFunctionLibrary|WidgetController")
	static UFFS_PlayerStatsWidgetController* GetWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilityBlueprintFunctionLibrary|WidgetController")
	static UFFS_AttributesWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|EnemyType")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEnemyType EnemyType, 
		float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static UEnemiesData* GetCharacterClassInfo(const UObject* WorldContextObject);
};

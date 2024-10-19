// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "FFS_AbilityBlueprintLibrary.generated.h"

class UFFS_PlayerStatsWidgetController;
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
};

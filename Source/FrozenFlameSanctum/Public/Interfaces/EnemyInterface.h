// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/EnemiesData.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROZENFLAMESANCTUM_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAttackTarget(AActor* InTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAttackTarget() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EEnemyType GetEnemyType();
};

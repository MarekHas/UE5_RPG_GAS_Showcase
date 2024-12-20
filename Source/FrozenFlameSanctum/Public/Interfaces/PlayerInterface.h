// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FROZENFLAMESANCTUM_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerLevel(int32 InPlayerLevel);
	UFUNCTION(BlueprintNativeEvent)
	void AddExperiencePoints(int32 InExperiencePoints);
	UFUNCTION(BlueprintNativeEvent)
	void AddSkillPoints(int32 InSkillPoints);
	UFUNCTION(BlueprintNativeEvent)
	void AddSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoints() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 CheckLevelForGivenExperience(int32 InExperiencePoints) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetExperiencePoints() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointsReceived(int32 Level) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReceived(int32 Level) const;

};

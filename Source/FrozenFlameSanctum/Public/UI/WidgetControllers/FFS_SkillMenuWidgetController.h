// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "FFS_SkillMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FROZENFLAMESANCTUM_API UFFS_SkillMenuWidgetController : public UFFS_WidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerLevelChangedSignature OnSkillPointsChangedDelegate;
};

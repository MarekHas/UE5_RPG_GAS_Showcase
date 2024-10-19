// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "FFS_AttributesWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AttributesWidgetController : public UFFS_WidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
};

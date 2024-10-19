// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FFS_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UFFS_AssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
	
};

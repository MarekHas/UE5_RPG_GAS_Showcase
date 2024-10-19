// Fill out your copyright notice in the Description page of Project Settings.


#include "FFS_AssetManager.h"
#include "FFS_GameplayTags.h"

UFFS_AssetManager& UFFS_AssetManager::Get()
{
	check(GEngine);

	UFFS_AssetManager* FFS_AssetManager = Cast<UFFS_AssetManager>(GEngine->AssetManager);
	return *FFS_AssetManager;
}

void UFFS_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FFFS_GameplayTags::InitializeNativeGameplayTags();
}

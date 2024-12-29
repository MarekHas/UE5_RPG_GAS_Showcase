// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HUD/FFS_GameHUD.h"

#include "UI/Widgets/FFS_UserWidget.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "UI/WidgetControllers/FFS_AttributesWidgetController.h"
#include "UI/WidgetControllers/FFS_SkillMenuWidgetController.h"

UFFS_PlayerStatsWidgetController* AFFS_GameHUD::GetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	if (PlayerStatsWidgetController == nullptr)
	{
		PlayerStatsWidgetController = NewObject<UFFS_PlayerStatsWidgetController>(this, PlayerStatsWidgetControllerClass);
		PlayerStatsWidgetController->SetWidgetControllerParams(WidgetControllerParams);

		return PlayerStatsWidgetController;
	}
	return PlayerStatsWidgetController;
}

UFFS_AttributesWidgetController* AFFS_GameHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UFFS_AttributesWidgetController>(this, AttributeMenuWidgetControllerClass);
		
		AttributeMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

UFFS_SkillMenuWidgetController* AFFS_GameHUD::GetSkillMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (SkillMenuWidgetController == nullptr)
	{
		SkillMenuWidgetController = NewObject<UFFS_SkillMenuWidgetController>(this, SkillMenuWidgetControllerClass);
		SkillMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		SkillMenuWidgetController->BindCallbacksToDependencies();
	}
	return SkillMenuWidgetController;
}

void AFFS_GameHUD::InitWidget(APlayerController* InController, APlayerState* InState, UAbilitySystemComponent* InAbilitySystem, UAttributeSet* InAttribute)
{
	checkf(MainCanvasClass, TEXT("MainCanvas Class uninitialized, please fill out BP_GameHUD"));
	checkf(PlayerStatsWidgetControllerClass, TEXT("PlayerStatsController Class uninitialized, please fill out BP_GameHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), MainCanvasClass);
	MainCanvas = Cast<UFFS_UserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(InController, InState, InAbilitySystem, InAttribute);
	PlayerStatsWidgetController = GetWidgetControllerParams(WidgetControllerParams);

	MainCanvas->SetWidgetController(PlayerStatsWidgetController);
	
	PlayerStatsWidgetController->BroadcastInitialValues();
	PlayerStatsWidgetController->BindCallbacksToDependencies();

	Widget->AddToViewport();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "Player/FFS_PlayerState.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "UI/HUD/FFS_GameHUD.h"

UFFS_PlayerStatsWidgetController* UFFS_AbilityBlueprintLibrary::GetWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFFS_GameHUD* GameHUD = Cast<AFFS_GameHUD>(PlayerController->GetHUD()))
		{
			AFFS_PlayerState* PlayerState = PlayerController->GetPlayerState<AFFS_PlayerState>();
			UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* Attributes = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystem, Attributes);

			return GameHUD->GetWidgetControllerParams(WidgetControllerParams);
		}
	}
	return nullptr;
}
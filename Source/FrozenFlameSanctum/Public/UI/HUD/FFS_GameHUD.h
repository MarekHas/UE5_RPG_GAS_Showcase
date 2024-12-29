// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FFS_GameHUD.generated.h"

class UFFS_SkillMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UFFS_UserWidget;
class UFFS_PlayerStatsWidgetController;
class UFFS_AttributesWidgetController;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_GameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFFS_PlayerStatsWidgetController* GetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);
	UFFS_AttributesWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	UFFS_SkillMenuWidgetController* GetSkillMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitWidget(APlayerController* InController, APlayerState* InState, UAbilitySystemComponent* InAbilitySystem, UAttributeSet* InAttribute);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFFS_UserWidget> MainCanvasClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFFS_PlayerStatsWidgetController> PlayerStatsWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UFFS_UserWidget>  MainCanvas;
	UPROPERTY()
	TObjectPtr<UFFS_PlayerStatsWidgetController> PlayerStatsWidgetController;

	UPROPERTY()
	TObjectPtr<UFFS_AttributesWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFFS_AttributesWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UFFS_SkillMenuWidgetController> SkillMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UFFS_SkillMenuWidgetController> SkillMenuWidgetControllerClass;
};

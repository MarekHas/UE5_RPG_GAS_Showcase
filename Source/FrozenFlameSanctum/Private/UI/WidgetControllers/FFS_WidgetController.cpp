// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_WidgetController.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "AbilitySystem/Data/AbilitiesInfo.h"
#include "Player/FFS_PlayerController.h"
#include "Player/FFS_PlayerState.h"

void UFFS_WidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UFFS_WidgetController::BroadcastInitialValues()
{
}

void UFFS_WidgetController::BindCallbacksToDependencies()
{
}

void UFFS_WidgetController::AbilityInfoBroadcast()
{
	if (!GetFFSAbilitySystemComponent()->bStartupAbilitiesGranted) return;

	FOnAbilityGiven BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(FFS_AbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = FFS_AbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		OnAbilityInfoFoundDelegate.Broadcast(Info);
	});
	GetFFSAbilitySystemComponent()->OnAbilityGiven(BroadcastDelegate);
}

AFFS_PlayerController* UFFS_WidgetController::GetFFSPlayerController()
{
	if(FFS_PlayerController == nullptr)
	{
		FFS_PlayerController = Cast<AFFS_PlayerController>(PlayerController);
	}
	return FFS_PlayerController;
}

AFFS_PlayerState* UFFS_WidgetController::GetFFSPlayerState()
{
	if(FFS_PlayerState == nullptr)
	{
		FFS_PlayerState = Cast<AFFS_PlayerState>(PlayerState);
	}
	return FFS_PlayerState;
}

UFFS_AbilitySystemComponent* UFFS_WidgetController::GetFFSAbilitySystemComponent()
{
	if(FFS_AbilitySystemComponent == nullptr)
	{
		FFS_AbilitySystemComponent = Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent);
	}
	return FFS_AbilitySystemComponent;
}

UFFS_AttributeSet* UFFS_WidgetController::GetFFSAttributeSet()
{
	if(FFS_AttributeSet == nullptr)
	{
		FFS_AttributeSet = Cast<UFFS_AttributeSet>(AttributeSet);
	}
	return FFS_AttributeSet;
}

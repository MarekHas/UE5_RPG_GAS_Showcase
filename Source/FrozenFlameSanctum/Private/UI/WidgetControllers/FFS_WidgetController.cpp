// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_WidgetController.h"

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

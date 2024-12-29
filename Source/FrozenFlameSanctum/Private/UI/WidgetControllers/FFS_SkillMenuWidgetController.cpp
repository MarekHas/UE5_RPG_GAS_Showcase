// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_SkillMenuWidgetController.h"

void UFFS_SkillMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	AbilityInfoBroadcast();
}

void UFFS_SkillMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}

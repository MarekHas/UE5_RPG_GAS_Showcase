// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_AttributesWidgetController.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "FFS_GameplayTags.h"
#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UFFS_AttributesWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetFFSAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}


	GetFFSPlayerState()->OnSkillPointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			SkillPointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UFFS_AttributesWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair : GetFFSAttributeSet()->TagsToAttributes) 
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	SkillPointsChangedDelegate.Broadcast(GetFFSPlayerState()->GetSkillPoints());
}

void UFFS_AttributesWidgetController::UpgradeSkill(const FGameplayTag& AttributeTag)
{
	GetFFSAbilitySystemComponent()->UpgradeSkill(AttributeTag);
}

void UFFS_AttributesWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FFFS_AttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

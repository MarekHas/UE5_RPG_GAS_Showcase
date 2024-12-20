// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_AttributesWidgetController.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "FFS_GameplayTags.h"
#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UFFS_AttributesWidgetController::BindCallbacksToDependencies()
{
	UFFS_AttributeSet* AS = CastChecked<UFFS_AttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	AFFS_PlayerState* FFS_PlayerState = CastChecked<AFFS_PlayerState>(PlayerState);
	FFS_PlayerState->OnSkillPointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			SkillPointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UFFS_AttributesWidgetController::BroadcastInitialValues()
{
	UFFS_AttributeSet* Attribute = CastChecked<UFFS_AttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : Attribute->TagsToAttributes) 
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	AFFS_PlayerState* FFS_PlayerState = CastChecked<AFFS_PlayerState>(PlayerState);
	SkillPointsChangedDelegate.Broadcast(FFS_PlayerState->GetSkillPoints());
}

void UFFS_AttributesWidgetController::UpgradeSkill(const FGameplayTag& AttributeTag)
{
	UFFS_AbilitySystemComponent* FFS_AbilitySystemComponent = CastChecked<UFFS_AbilitySystemComponent>(AbilitySystemComponent);
	FFS_AbilitySystemComponent->UpgradeSkill(AttributeTag);
}

void UFFS_AttributesWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FFFS_AttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

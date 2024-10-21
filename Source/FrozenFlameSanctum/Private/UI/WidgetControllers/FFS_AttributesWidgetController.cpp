// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetControllers/FFS_AttributesWidgetController.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "FFS_GameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/FFS_AttributeSet.h"

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
}

void UFFS_AttributesWidgetController::BroadcastInitialValues()
{
	UFFS_AttributeSet* Attribute = CastChecked<UFFS_AttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : Attribute->TagsToAttributes) 
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UFFS_AttributesWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FFFS_AttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

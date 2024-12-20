// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/FFS_PlayerState.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "FFS_AttributesWidgetController.generated.h"

class UAttributeInfo;
struct FFFS_AttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const struct FFFS_AttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPointsChangedSignature, int32, NewValue);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FROZENFLAMESANCTUM_API UFFS_AttributesWidgetController : public UFFS_WidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerPointsChangedSignature SkillPointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
    void UpgradeSkill(const FGameplayTag& AttributeTag);
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};

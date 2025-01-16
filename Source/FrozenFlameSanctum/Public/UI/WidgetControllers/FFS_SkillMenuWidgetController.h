// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFS_GameplayTags.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "FFS_SkillMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSkillButtonSelectedSignature,
	bool, bCanSpendSkillPoint,
	bool, bCanEquipWithSkill,
	FString, AbilityDescription,
	FString, LevelUpAbilityDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipSelectionChangedSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillReassignedSignature, const FGameplayTag&, AbilityTag);
struct FSelectedSkillTags
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag State = FGameplayTag();
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class FROZENFLAMESANCTUM_API UFFS_SkillMenuWidgetController : public UFFS_WidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerLevelChangedSignature OnSkillPointsChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEquipSelectionChangedSignature OnEquipStartedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEquipSelectionChangedSignature OnEquipEndedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSkillReassignedSignature OnSkillReassignedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSkillButtonSelectedSignature OnSkillButtonSelectedDelegate;
	UFUNCTION(BlueprintCallable)
	void SkillButtonSelected(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	UFUNCTION(BlueprintCallable)
    void DeselectSkill();

	UFUNCTION(BlueprintCallable)
	void SkillPressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityState,
		const FGameplayTag& CurrentInputTag, const FGameplayTag& PreviousInputTag);
private:
	FSelectedSkillTags SelectedSkill = { FFFS_GameplayTags::Get().Ability_Type_None,  FFFS_GameplayTags::Get().Ability_State_Locked };
	int32 CurrentSkillPoints = 0;
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bCanSpendSkillPoint, bool& bCanEquipWithSkill);
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlotTag;
};

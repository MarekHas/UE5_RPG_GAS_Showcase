// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "CooldownEffectAsyncAction.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);
/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class FROZENFLAMESANCTUM_API UCooldownEffectAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UCooldownEffectAsyncAction* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayTag& InCooldownTag);
	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	FGameplayTag CooldownTag;
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetAbilitySystemComponent, const FGameplayEffectSpec& SpecApplied,
		FActiveGameplayEffectHandle ActiveEffectHandle);
};

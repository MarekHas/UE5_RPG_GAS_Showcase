// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CooldownEffectAsyncAction.h"
#include "AbilitySystemComponent.h"

UCooldownEffectAsyncAction* UCooldownEffectAsyncAction::WaitForCooldownChange(
	UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UCooldownEffectAsyncAction* WaitCooldownChange = NewObject<UCooldownEffectAsyncAction>();
	WaitCooldownChange->AbilitySystemComponent = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	
	// To know when a cooldown has ended (Cooldown Tag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UCooldownEffectAsyncAction::CooldownTagChanged);
	
	// To know when a cooldown effect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCooldownChange,
		&UCooldownEffectAsyncAction::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UCooldownEffectAsyncAction::EndTask()
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCooldownEffectAsyncAction::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UCooldownEffectAsyncAction::OnActiveEffectAdded(UAbilitySystemComponent* TargetAbilitySystemComponent,
                                                     const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}

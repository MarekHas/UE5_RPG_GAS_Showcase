// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "FFS_AbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectAppliedSignature, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FOnAbilitiesGrantedSignature);
DECLARE_DELEGATE_OneParam(FOnAbilityGiven, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAbilityStateChangedSignature , const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*AbilityStateTag*/)
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FOnEffectAppliedSignature OnEffectAppliedDelegate;
	FOnAbilitiesGrantedSignature OnAbilitiesGrantedDelegate;
	FOnAbilityStateChangedSignature  OnAbilityStateChangedDelegate;
	
	//This function should be called affter InitAbilityActorInfo 
	void BindToAbilitySystemDelegates();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void OnAbilityGiven(const FOnAbilityGiven& Delegate);
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetAbilityStateFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpgradeSkill(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void Server_UpgradeSkill(const FGameplayTag& AttributeTag);
	void UpdateAbilityState(int32 Level);
	bool bStartupAbilitiesGranted = false;
protected:
	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION(Client, Reliable)
	void Client_OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityState(const FGameplayTag& AbilityTag, const FGameplayTag& StateTag);
};

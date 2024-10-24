// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/FFS_InputSettings.h"

const UInputAction* UFFS_InputSettings::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FInputActionGameplayTagPair& Pair : AbilityInputActions)
	{
		if (Pair.InputAction && Pair.InputTag == InputTag)
		{
			return Pair.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

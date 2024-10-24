// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/FFS_InputSettings.h"

#include "FFS_InputComponent.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UFFS_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	
	void BindAbilityActions(const UFFS_InputSettings* InputConfig, UserClass* Object, PressedFuncType PressedFunc, 
		ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UFFS_InputComponent::BindAbilityActions(const UFFS_InputSettings* InputSettings, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputSettings);
	for (const FInputActionGameplayTagPair& Pair : InputSettings->AbilityInputActions)
	{
		if (Pair.InputAction && Pair.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Pair.InputAction, ETriggerEvent::Started, Object, PressedFunc, Pair.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Pair.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Pair.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Pair.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Pair.InputTag);
			}
		}
	}
}
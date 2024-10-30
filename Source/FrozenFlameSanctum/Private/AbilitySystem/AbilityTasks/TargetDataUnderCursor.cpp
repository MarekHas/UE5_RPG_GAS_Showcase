// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCursor* DataUnderCursor = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return DataUnderCursor;
}

void UTargetDataUnderCursor::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendUnderCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderCursor::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderCursor::SendUnderCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle AbilityTargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* GameplayAbilityTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	GameplayAbilityTargetData->HitResult = CursorHit;
	AbilityTargetDataHandle.Add(GameplayAbilityTargetData);

	AbilitySystemComponent->ServerSetReplicatedTargetData
	(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		AbilityTargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TargetDataUnderCursorDelegate.Broadcast(AbilityTargetDataHandle);
	}
}

void UTargetDataUnderCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TargetDataUnderCursorDelegate.Broadcast(DataHandle);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

#include "FFS_AbilityTypes.h"
#include "Game/FFS_GameModeBase.h"
#include "Player/FFS_PlayerState.h"
#include "UI/HUD/FFS_GameHUD.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "AbilitySystem/Data/EnemiesData.h"
#include "Interfaces/CombatInterface.h"

UFFS_PlayerStatsWidgetController* UFFS_AbilityBlueprintLibrary::GetWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFFS_GameHUD* GameHUD = Cast<AFFS_GameHUD>(PlayerController->GetHUD()))
		{
			AFFS_PlayerState* PlayerState = PlayerController->GetPlayerState<AFFS_PlayerState>();
			UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* Attributes = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystem, Attributes);

			return GameHUD->GetWidgetControllerParams(WidgetControllerParams);
		}
	}
	return nullptr;
}

UFFS_AttributesWidgetController* UFFS_AbilityBlueprintLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFFS_GameHUD* GameHUD = Cast<AFFS_GameHUD>(PlayerController->GetHUD()))
		{
			AFFS_PlayerState* PlayerState = PlayerController->GetPlayerState<AFFS_PlayerState>();
			UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* Attributes = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystem, Attributes);

			return GameHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UFFS_AbilityBlueprintLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EEnemyType EnemyType, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	AFFS_GameModeBase* FFS_GameMode = Cast<AFFS_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FFS_GameMode == nullptr) return;

	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
	UEnemiesData* EnemiesData = GetCharacterClassInfo(WorldContextObject);
	FEnemyDefaultStats EnemyStats = EnemiesData->GetClassDefaultInfo(EnemyType);
	
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EnemyStats.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EnemiesData->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EnemiesData->VitalAttributes, Level, VitalAttributesContextHandle);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, EEnemyType EnemyType)
{
	UEnemiesData* EnemiesData = GetCharacterClassInfo(WorldContextObject);
	if(EnemiesData == nullptr) return;
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : EnemiesData->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	const FEnemyDefaultStats& DefaultInfo = EnemiesData->GetClassDefaultInfo(EnemyType);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (AbilitySystemComponent->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(AbilitySystemComponent->GetAvatarActor()));
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

UEnemiesData* UFFS_AbilityBlueprintLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AFFS_GameModeBase* FFS_GameMode = Cast<AFFS_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FFS_GameMode == nullptr) return nullptr;
	return FFS_GameMode->EnemiesData;
}

void UFFS_AbilityBlueprintLibrary::GetLivePlayersInRange(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Range, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Range),
			SphereParams);
		
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}

bool UFFS_AbilityBlueprintLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFFS_GameplayEffectContext* FFS_GameplayEffectContext = static_cast<const FFFS_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FFS_GameplayEffectContext->IsBlockedHit();
	}
	return false;
}

bool UFFS_AbilityBlueprintLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFFS_GameplayEffectContext* FFS_GameplayEffectContext = static_cast<const FFFS_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FFS_GameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UFFS_AbilityBlueprintLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                   const bool bInIsBlockedHit)
{
	if (FFFS_GameplayEffectContext* FFS_GameplayEffectContext = static_cast<FFFS_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FFS_GameplayEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UFFS_AbilityBlueprintLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                    const bool bInIsCriticalHit)
{
	if (FFFS_GameplayEffectContext* FFS_GameplayEffectContext = static_cast<FFFS_GameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FFS_GameplayEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

int32 UFFS_AbilityBlueprintLibrary::ExperiencePointsForKilledEnemy(const UObject* WorldContextObject,
	EEnemyType EnemyType, int32 Level)
{
	UEnemiesData* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;
	const FEnemyDefaultStats& Info = CharacterClassInfo->GetClassDefaultInfo(EnemyType);
	const float XPReward = Info.ExperiencePointsReward.GetValueAtLevel(Level);
	return static_cast<int32>(XPReward);
}

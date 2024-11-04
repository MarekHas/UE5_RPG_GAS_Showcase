// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FFS_AbilityBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Game/FFS_GameModeBase.h"
#include "Player/FFS_PlayerState.h"
#include "UI/HUD/FFS_GameHUD.h"
#include "UI/WidgetControllers/FFS_WidgetController.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "AbilitySystem/Data/EnemiesData.h"

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

	AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
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

void UFFS_AbilityBlueprintLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent)
{
	AFFS_GameModeBase* FFS_GameMode = Cast<AFFS_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FFS_GameMode == nullptr) return;
	
	UEnemiesData* EnemiesData = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : EnemiesData->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

UEnemiesData* UFFS_AbilityBlueprintLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AFFS_GameModeBase* FFS_GameMode = Cast<AFFS_GameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FFS_GameMode == nullptr) return nullptr;
	return FFS_GameMode->EnemiesData;
}

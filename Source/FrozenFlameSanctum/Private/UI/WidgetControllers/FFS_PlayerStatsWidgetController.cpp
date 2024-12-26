// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "AbilitySystem/Data/CharacterProgressData.h"
#include "Player/FFS_PlayerState.h"

void UFFS_PlayerStatsWidgetController::BroadcastInitialValues()
{
	const UFFS_AttributeSet* FFS_AttributeSet = CastChecked<UFFS_AttributeSet>(AttributeSet);

	OnHealthChangedDelegate.Broadcast(FFS_AttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(FFS_AttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(FFS_AttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(FFS_AttributeSet->GetMaxMana());
}

void UFFS_PlayerStatsWidgetController::BindCallbacksToDependencies()
{
	AFFS_PlayerState* FFS_PlayerState = CastChecked<AFFS_PlayerState>(PlayerState);
	FFS_PlayerState->OnExperiencePointsChangedDelegate.AddUObject(this, &UFFS_PlayerStatsWidgetController::OnExperiencePointsChanged);
	FFS_PlayerState->OnPlayerLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel){OnPlayerLevelChangedDelegate.Broadcast(NewLevel);});
	
	const UFFS_AttributeSet* FFS_AttributeSet = CastChecked<UFFS_AttributeSet>(AttributeSet);
	//Player Stats delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnHealthChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChangedDelegate.Broadcast(Data.NewValue); });
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnManaChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FFS_AttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxManaChangedDelegate.Broadcast(Data.NewValue); });

	if(UFFS_AbilitySystemComponent* FFS_AbilitySystemComponent = Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent))
	{
		if(FFS_AbilitySystemComponent->bStartupAbilitiesGranted)
		{
			OnInitializeStartupAbilities(FFS_AbilitySystemComponent);
		}
		else
		{
			FFS_AbilitySystemComponent->OnAbilitiesGrantedDelegate.AddUObject(this,&UFFS_PlayerStatsWidgetController::OnInitializeStartupAbilities);	
		}

		FFS_AbilitySystemComponent->OnEffectAppliedDelegate.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Notification"));
					if (Tag.MatchesTag(MessageTag))
					{
						if (NotificationWidgetsDataTable == nullptr)
						{
							GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "DT_Notficatins NULLPTR");
							return;
						}

						const FNotificationWidgetRow* Row = GetDataTableRowByTag<FNotificationWidgetRow>(
							NotificationWidgetsDataTable, Tag);
						if (Row == nullptr)
						{
							GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "ROW NULLPTR");
							return;
						}
						OnNotificationDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
	
	//Notification about effect applied delegate
	Cast<UFFS_AbilitySystemComponent>(AbilitySystemComponent)->OnEffectAppliedDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Notification"));
				if (Tag.MatchesTag(MessageTag)) 
				{
					if (NotificationWidgetsDataTable == nullptr) 
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "DT_Notficatins NULLPTR");
						return;
					}

					const FNotificationWidgetRow* Row = GetDataTableRowByTag<FNotificationWidgetRow>(NotificationWidgetsDataTable, Tag);
					if (Row == nullptr)
					{
						GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "ROW NULLPTR");
						return;
					}
					OnNotificationDelegate.Broadcast(*Row);
				}
			}
		}
	);
}

void UFFS_PlayerStatsWidgetController::OnInitializeStartupAbilities(
	UFFS_AbilitySystemComponent* FFS_AbilitySystemComponent)
{
	if (!FFS_AbilitySystemComponent->bStartupAbilitiesGranted) return;

	FOnAbilityGiven BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, FFS_AbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FFFS_AbilityInfo Info = AbilitiesInfo->FindAbilityInfoForTag(FFS_AbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = FFS_AbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		OnAbilityInfoFoundDelegate.Broadcast(Info);
	});
	FFS_AbilitySystemComponent->OnAbilityGiven(BroadcastDelegate);
}

void UFFS_PlayerStatsWidgetController::OnExperiencePointsChanged(int32 NewExperiencePoints) const
{
	AFFS_PlayerState* FFS_PlayerState = CastChecked<AFFS_PlayerState>(PlayerState);
	UCharacterProgressData* ProgressData = FFS_PlayerState->CharacterProgressData;

	checkf(ProgressData, TEXT("Unable to find ProgressData. Missing on PlayerState Blueprint"));

	const int32 CurrentLevel = ProgressData->GetCharacterLevel(NewExperiencePoints);
	const int32 MaxLevel = ProgressData->LevelUpData.Num();

	if (CurrentLevel <= MaxLevel && CurrentLevel > 0)
	{
		const int32 LevelUpPointsRequired = ProgressData->LevelUpData[CurrentLevel].ExperiencePointsRequired;
		const int32 PreviousLevelUpPointsRequired = ProgressData->LevelUpData[CurrentLevel - 1].ExperiencePointsRequired;
		
		const int32 DeltaPointsRequired = LevelUpPointsRequired - PreviousLevelUpPointsRequired;
		const int32 ActualPointsOnCurrentLevel = NewExperiencePoints - PreviousLevelUpPointsRequired;
		const float PercentOfExperience = static_cast<float>(ActualPointsOnCurrentLevel) / static_cast<float>(DeltaPointsRequired);

		OnExperiencePointsChangedDelegate.Broadcast(PercentOfExperience);
	}
}

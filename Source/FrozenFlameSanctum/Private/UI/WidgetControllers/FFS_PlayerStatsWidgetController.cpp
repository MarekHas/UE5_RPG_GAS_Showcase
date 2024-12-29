// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"

#include "AbilitySystem/FFS_AbilitySystemComponent.h"
#include "AbilitySystem/FFS_AttributeSet.h"
#include "AbilitySystem/Data/CharacterProgressData.h"
#include "Player/FFS_PlayerState.h"

void UFFS_PlayerStatsWidgetController::BroadcastInitialValues()
{
	OnHealthChangedDelegate.Broadcast(GetFFSAttributeSet()->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(GetFFSAttributeSet()->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(GetFFSAttributeSet()->GetMana());
	OnMaxManaChangedDelegate.Broadcast(GetFFSAttributeSet()->GetMaxMana());
}

void UFFS_PlayerStatsWidgetController::BindCallbacksToDependencies()
{
	GetFFSPlayerState()->OnExperiencePointsChangedDelegate.AddUObject(this, &UFFS_PlayerStatsWidgetController::OnExperiencePointsChanged);
	GetFFSPlayerState()->OnPlayerLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel){OnPlayerLevelChangedDelegate.Broadcast(NewLevel);});
	
	//Player Stats delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetFFSAttributeSet()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnHealthChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetFFSAttributeSet()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChangedDelegate.Broadcast(Data.NewValue); });
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetFFSAttributeSet()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnManaChangedDelegate.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetFFSAttributeSet()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxManaChangedDelegate.Broadcast(Data.NewValue); });

	if(GetFFSAbilitySystemComponent())
	{
		if(GetFFSAbilitySystemComponent()->bStartupAbilitiesGranted)
		{
			AbilityInfoBroadcast();
		}
		else
		{
			GetFFSAbilitySystemComponent()->OnAbilitiesGrantedDelegate.AddUObject(this,&UFFS_WidgetController::AbilityInfoBroadcast);	
		}

		GetFFSAbilitySystemComponent()->OnEffectAppliedDelegate.AddLambda(
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
	GetFFSAbilitySystemComponent()->OnEffectAppliedDelegate.AddLambda(
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

void UFFS_PlayerStatsWidgetController::OnExperiencePointsChanged(int32 NewExperiencePoints)
{
	UCharacterProgressData* ProgressData = GetFFSPlayerState()->CharacterProgressData;

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

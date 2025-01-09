// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "FFS_PlayerState.generated.h"

class UCharacterProgressData;
class UAbilitySystemComponent;
class UAttributeSet;

/*changed value on player state it could be level or experience points*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, int32)
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AFFS_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterProgressData> CharacterProgressData;
	
	FOnPlayerStateChangedSignature OnPlayerLevelChangedDelegate;
	FOnPlayerStateChangedSignature OnExperiencePointsChangedDelegate;
	FOnPlayerStateChangedSignature OnAttributePointsChangedDelegate;
	FOnPlayerStateChangedSignature OnSkillPointsChangedDelegate;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	FORCEINLINE int32 GetExperiencePoints() const { return ExperiencePoints; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }
	
	void AddExperiencePoints(const int32 InPoints);
	void SetExperiencePoints(const int32 InPoints);
	void AddAttributePoints(int32 InPoints);
	void AddSkillPoints(int32 InPoints);
	
	void LevelUp(const int32 InLevel);
	void SetLevel(const int32 InLevel);
protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 PlayerLevel = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ExperiencePoints)
	int32 ExperiencePoints = 1;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SkillPoints)
	int32 SkillPoints = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_ExperiencePoints(int32 OldExperiencePoints);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SkillPoints(int32 OldSkillPoints);
	
	void SetupAbilitySystemComponent();
};

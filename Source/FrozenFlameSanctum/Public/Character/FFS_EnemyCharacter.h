// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/FFS_CharacterBase.h"
#include "Interfaces/MarkableInterface.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "AbilitySystem/Data/EnemiesData.h"

#include "FFS_EnemyCharacter.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_EnemyCharacter : public AFFS_CharacterBase, public IMarkableInterface
{
	GENERATED_BODY()
	
public:
	AFFS_EnemyCharacter();

	// Begin IMarkableInterface
	void MarkActor() override;
	void UnmarkActor() override;
	// end IMarkableInterface
	
	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatsChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatsChangedSignature OnMaxHealthChanged;
protected:
	virtual void BeginPlay() override;
	//~Begin AFFS_CharacterBase override
	virtual void InitAbilityActorInfo() override;
	//~End  AFFS_CharacterBase override
	virtual void InitDefaultStats() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy stats")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	EEnemyType EnemyType = EEnemyType::Melee;
private:
	void SetupAbilitySystemComponent();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/FFS_CharacterBase.h"

#include "Interfaces/EnemyInterface.h"
#include "Interfaces/MarkableInterface.h"
#include "UI/WidgetControllers/FFS_PlayerStatsWidgetController.h"
#include "AbilitySystem/Data/EnemiesData.h"

#include "FFS_EnemyCharacter.generated.h"

class AFFS_AIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_EnemyCharacter : public AFFS_CharacterBase, public IEnemyInterface, public IMarkableInterface
{
	GENERATED_BODY()
	
public:
	AFFS_EnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;

	// Begin IEnemyInterface
	virtual void SetAttackTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetAttackTarget_Implementation() const override;
	//End IEnemyInterface
	
	// Begin IMarkableInterface
	void MarkActor() override;
	void UnmarkActor() override;
	// end IMarkableInterface
	
	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Death() override;
	/** end Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatsChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatsChangedSignature OnMaxHealthChanged;

	void HitReactionTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeTime = 5.f;
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> AttackTarget;
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

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AFFS_AIController> FFS_AIController;
private:
	void SetupAbilitySystemComponent();
};

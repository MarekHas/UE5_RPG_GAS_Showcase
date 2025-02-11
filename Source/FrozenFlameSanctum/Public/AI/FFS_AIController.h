// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "FFS_AIController.generated.h"

struct FAIStimulus;
class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API AFFS_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AFFS_AIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;
	
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Crowd Avoidance Config")
	bool bEnableCrowdAvoidance = true;
	UPROPERTY(EditDefaultsOnly, Category = "Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance",UIMin = "1",UIMax = "4"))
	int32 CrowdAvoidanceQuality = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Crowd Avoidance Config", meta = (EditCondition = "bEnableCrowdAvoidance"))
	float CollisionQueryRange = 500.f;
};

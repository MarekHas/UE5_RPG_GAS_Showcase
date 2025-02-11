// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FacingToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UBTS_FacingToTargetActor : public UBTService
{
	GENERATED_BODY()

	UBTS_FacingToTargetActor();
	
	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationSpeed;
};

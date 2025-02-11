// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FaceTargetRotation.generated.h"

struct FOwningPawnTargetActorPair
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;
	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}
	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class FROZENFLAMESANCTUM_API UBTT_FaceTargetRotation : public UBTTaskNode
{
	GENERATED_BODY()

	UBTT_FaceTargetRotation();
	
	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	bool HasReachedPrecisionAngle(APawn* OwningPawn,AActor* TargetActor) const;
	
	UPROPERTY(EditAnywhere, Category = "Rotate to Target")
	float PrecisionAngle;
	UPROPERTY(EditAnywhere, Category = "Rotate to Target")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, Category = "Rotate to Target")
	FBlackboardKeySelector InFaceTargetKey;
};

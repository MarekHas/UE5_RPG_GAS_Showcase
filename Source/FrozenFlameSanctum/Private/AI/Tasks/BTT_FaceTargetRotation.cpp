// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTT_FaceTargetRotation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_FaceTargetRotation::UBTT_FaceTargetRotation()
{
	NodeName = TEXT("Rotate to Face Target Actor");
	PrecisionAngle = 10.f;
	RotationSpeed = 5.f;
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	INIT_TASK_NODE_NOTIFY_FLAGS();
	InFaceTargetKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass,InFaceTargetKey),AActor::StaticClass());
}

void UBTT_FaceTargetRotation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InFaceTargetKey.ResolveSelectedKey(*BBAsset);
	}
}

uint16 UBTT_FaceTargetRotation::GetInstanceMemorySize() const
{
	return sizeof(FOwningPawnTargetActorPair);
}

FString UBTT_FaceTargetRotation::GetStaticDescription() const
{
	const FString KeyDescription = InFaceTargetKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Rotates to face %s Key until the angle precision: %s is reached"),
		*KeyDescription,*FString::SanitizeFloat(PrecisionAngle));
}

EBTNodeResult::Type UBTT_FaceTargetRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InFaceTargetKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	FOwningPawnTargetActorPair* Memory = CastInstanceNodeMemory<FOwningPawnTargetActorPair>(NodeMemory);
	check(Memory);
	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;
	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}
	if (HasReachedPrecisionAngle(OwningPawn,TargetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::InProgress;
}

void UBTT_FaceTargetRotation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FOwningPawnTargetActorPair* Memory = CastInstanceNodeMemory<FOwningPawnTargetActorPair>(NodeMemory);

	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	}

	if (HasReachedPrecisionAngle(Memory->OwningPawn.Get(),Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(),Memory->TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(),LookAtRot,DeltaSeconds,RotationSpeed);
		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

bool UBTT_FaceTargetRotation::HasReachedPrecisionAngle(APawn* OwningPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = OwningPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - OwningPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward,OwnerToTargetNormalized);
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	return AngleDiff <= PrecisionAngle;
}

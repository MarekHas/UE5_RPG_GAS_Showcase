// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS_FacingToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_FacingToTargetActor::UBTS_FacingToTargetActor()
{
	NodeName = TEXT("Native Facing Rotation To Target Actor");
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	RotationSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass,InTargetActorKey),AActor::StaticClass());
}

void UBTS_FacingToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTS_FacingToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Face rotation to %s Key %s"),*KeyDescription,*GetStaticServiceDescription());
}

void UBTS_FacingToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(),TargetActor->GetActorLocation());
		const FRotator TargetRotator = FMath::RInterpTo(OwningPawn->GetActorRotation(),LookAtRotator,DeltaSeconds,RotationSpeed);
		OwningPawn->SetActorRotation(TargetRotator);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FFS_AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AFFS_AIController::AFFS_AIController(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	check(AISenseConfig_Sight);
	
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 3000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 150.f;
	
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	check(EnemyPerceptionComponent);
	
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AFFS_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AFFS_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdFollowingComponent->SetCrowdSimulationState(bEnableCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (CrowdAvoidanceQuality)
		{
		case 1: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);
			break;
		case 2: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);
			break;
		case 3: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
			break;
		case 4: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
			break;
		default:
			break;
		}
		
		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AFFS_AIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetToAttack")))
		{
			if(Stimulus.WasSuccessfullySensed() && Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetToAttack"), Actor);
			}
		}
	}
}

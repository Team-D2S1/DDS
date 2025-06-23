// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/Player/DDSPlayerState.h"
#include "ETC/CustomLog.h"
#include "ETC/Enum.h"

AAIControllerBase::AAIControllerBase(FObjectInitializer const& ObjectInitializer)
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SetPerceptionComponent(*AIPerceptionComponent);

	AAIController::SetGenericTeamId(FGenericTeamId(static_cast<uint8>(EGameTeam::Monster)));
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = nullptr;
	if (OtherPawn)
	{
		OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
		if (!OtherTeamAgent)
		{
			OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetPlayerState());
		}
	}
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId().GetId() != GetGenericTeamId().GetId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
	}
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Actor) return;
	
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if(Stimulus.WasSuccessfullySensed() && Actor && GetTeamAttitudeTowards(*Actor) == ETeamAttitude::Hostile)
		{
			BlackboardComponent->SetValueAsObject("TargetActor", Actor);
			
		}
		else
		{
			BlackboardComponent->SetValueAsVector("LastSeenLocation", Actor->GetActorLocation());
			BlackboardComponent->SetValueAsBool("bIsInvestigating", true);
			BlackboardComponent->SetValueAsObject("TargetActor", nullptr);

			MY_LOG(LogTemp, Error, TEXT("Lost Target"));
		}
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	OriginLocation = InPawn->GetActorLocation();
		
	// Run BehaviorTree
	if(BehaviorTree && BlackBoardData)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

bool AAIControllerBase::InitializeBlackboard(UBlackboardComponent& BlackboardComp, UBlackboardData& BlackboardAsset)
{
	bool bSuccess = Super::InitializeBlackboard(BlackboardComp, BlackboardAsset);

	if(bSuccess)
	{
		BlackboardComp.SetValueAsFloat("AttackRange", AttackRange);
		// TODO
		// OriginLocation 적용 되는지 확인하기
		BlackboardComp.SetValueAsVector("OriginLocation", GetPawn()->GetActorLocation());
	}

	return bSuccess;
}

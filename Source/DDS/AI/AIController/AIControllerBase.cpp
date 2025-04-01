// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AIControllerBase.h"

#include "AI/DDSPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase(FObjectInitializer const& ObjectInitializer)
{
	AIPerception = CreateDefaultSubobject<UDDSPerceptionComponent>(TEXT("AI Perception"));
	SetPerceptionComponent(*AIPerception);

	SetGenericTeamId(FGenericTeamId(1));
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}

ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn);
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId().GetId() != GetGenericTeamId().GetId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);
	}
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Set AbilityComponent
	if(AMonsterBase* Monster = Cast<AMonsterBase>(InPawn))
	{
		AbilityComponent = Monster->GetAbilitySystemComponent();
		OriginPosition = Monster->GetActorLocation();
	}

	// Run BehaviorTree
	if(BehaviorTree && BlackBoardData)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

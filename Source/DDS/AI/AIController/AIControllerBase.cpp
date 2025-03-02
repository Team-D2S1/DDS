// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AIControllerBase.h"

#include "Character/Monster/MonsterBase.h"
#include "ETC/CustomLog.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIControllerBase::AAIControllerBase(FObjectInitializer const& ObjectInitializer)
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SetPerceptionComponent(*AIPerception);
	SetupPerception();
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	InitPerception();
}

void AAIControllerBase::SetupPerception()
{
	// 전방 시야 설정
	Config_FrontSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Front Sight"));
	Config_FrontSight->PeripheralVisionAngleDegrees  = 60.f;
	Config_FrontSight->DetectionByAffiliation.bDetectEnemies = true;
	Config_FrontSight->DetectionByAffiliation.bDetectFriendlies = true;
	Config_FrontSight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*Config_FrontSight);
	AIPerception->SetDominantSense(Config_FrontSight->GetSenseImplementation());
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}

void AAIControllerBase::InitPerception()
{
	Config_FrontSight->SightRadius = FrontDetectionRange;
	Config_FrontSight->LoseSightRadius = FrontDetectionRange + 100.f;
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(Stimulus.WasSuccessfullySensed())
	{
		MY_LOG(LogTemp, Warning, TEXT("AI 감지 : %s"), *Actor->GetName());
	}
	else
	{
		MY_LOG(LogTemp, Warning, TEXT("AI 감지 상실 : %s"), *Actor->GetName());
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

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Service/BTService_UpdateDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_UpdateDistance::UBTService_UpdateDistance()
{
	NodeName = "UpdateDistance";
}

void UBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Initialize
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));

	// 거리 측정 및 기록
	const float Distance = FVector::Dist(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Target->GetActorLocation());
	Blackboard->SetValueAsFloat("CurrentDistance", Distance);
}

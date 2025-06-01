// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Service/BTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"


// Monster에서만 불리는 함수
void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		
		AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
		// 아마 이건 없애도 될 듯?
		if(!Target) return;

		float Distance = FVector::Dist(Monster->GetActorLocation(), Target->GetActorLocation());
		float AttackRange = Monster->GetAttackRange();

		bool bCanAttack = Distance <= AttackRange;
		Blackboard->SetValueAsBool("bCanAttack", bCanAttack);
	}
}

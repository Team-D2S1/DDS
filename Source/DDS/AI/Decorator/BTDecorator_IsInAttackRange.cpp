// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Decorator/BTDecorator_IsInAttackRange.h"

#include "AIController.h"
#include "AI/AIController/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ETC/CustomLog.h"
#include "GameFramework/Character.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = "Is In Attack Range";
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	MY_LOG(LogTemp, Error, TEXT("거리 판정"));
	
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!MyPawn) return false;
	
	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	if(!Target) return false;

	float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AttackRange");

	if(AttackRangeType == EAttackRangeType::Normal)
	{
		if(FVector::Dist(MyPawn->GetActorLocation(), Target->GetActorLocation()) <= AttackRange)
		{
			return true;
		}
	}
	
	return false;
}


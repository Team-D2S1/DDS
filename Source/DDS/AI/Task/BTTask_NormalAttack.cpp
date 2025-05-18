// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Task/BTTask_NormalAttack.h"

#include "AI/AIController/AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "ETC/CustomLog.h"


UBTTask_NormalAttack::UBTTask_NormalAttack()
{
	NodeName = "Normal Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_NormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MY_LOG(LogTemp, Log, TEXT("Execute Monster Attack Sequence"));

	CurrentAttackSequence = 0;
	bIsFinished = false;
	
	AMonsterBase* OwnerMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!OwnerMonster) return EBTNodeResult::Failed;
	
	OwnerMonster->GetMonsterCombatComponent()->Attack();
	CurrentAttackSequence++;

	return EBTNodeResult::InProgress;
}

void UBTTask_NormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	/*
	 * [TickTask 사용 이유]
	 * 공격 태스크는 공격 애니메이션이 끝날 때까지 대기해야 하는 지연 테스트이다
	 * 따라서 ExecuteTask의 결과값을 InProgress로 반환하고, 공격이 끝났을 때 Task가 완료되었다고 알려야 한다
	 * 이를 위해 FinishLatentTask 함수를 사용한다
	 * 해당 함수를 사용하지 않으면 행동트리 시스템은 현재 테스크에 계속 머무른다
	 */
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(bIsFinished) return;

	ElapsedTime += DeltaSeconds;
	if(ElapsedTime < AttackInterval) return;

	ElapsedTime = 0.f;
	
	AMonsterBase* OwnerMonster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!OwnerMonster) return;

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(("TargetActor")));
	if(!Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		bIsFinished = true;
		return;
	}
	
	// FIXME
	// - 현재 공격이 타겟에게 닿는지 확인하는 과정이 좀 더 확실할 필요가 있다
	float AttackRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AttackRange");
	float Distance = FVector::Dist(OwnerMonster->GetActorLocation(), Target->GetActorLocation());
	if(Distance > AttackRange)
	{
		MY_LOG(LogTemp, Log, TEXT("타겟이 범위 밖으로 이동해서 공격 시퀀스 중지"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		bIsFinished = true;
		return;
	}

	// TODO
	// Attack 함수 수정 필요
	OwnerMonster->GetMonsterCombatComponent()->Attack();
	CurrentAttackSequence++;

	if(CurrentAttackSequence >= MaxAttackSequence)
	{
		MY_LOG(LogTemp, Log, TEXT("공격 시퀀스 종료"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		bIsFinished = true;
	}
}

void UBTTask_NormalAttack::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

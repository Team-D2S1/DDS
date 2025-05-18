// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UBTTask_NormalAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_NormalAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (ClampMin = "1", ExposeOnSpawn = true))
	int32 MaxAttackSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = (ExposeOnSpawn = true))
	float AttackInterval = 1.f;

private:
	int32 CurrentAttackSequence = 0;
	float ElapsedTime = 0.f;
	// 유효성 있는가?
	bool bIsFinished = false;
};

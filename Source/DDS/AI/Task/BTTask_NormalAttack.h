// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NormalAttack.generated.h"

class UGameplayAbility;
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

private:
	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* Ability);

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;

	FDelegateHandle OnAbilityEndDelegateHandle;
};

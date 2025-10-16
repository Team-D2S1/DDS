// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "BTService_SetSkill.generated.h"


struct FCurrentSkillInfo;
class UMonsterSkillBase;

UCLASS()
class DDS_API UBTService_SetSkill : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UMonsterSkillBase* GetRandomMonsterSkillWithWeight(const TArray<FCurrentSkillInfo>& UsableSkills);
};

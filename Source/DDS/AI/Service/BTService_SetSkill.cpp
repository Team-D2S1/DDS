// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Service/BTService_SetSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Character/Player/DDSPlayerState.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "ETC/CustomLog.h"


void UBTService_SetSkill::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Monster) return;

	UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	if(Blackboard->GetValueAsBool("bIsUsingSkill") == true) return;

	float CurrentDistance = Blackboard->GetValueAsFloat("CurrentDistance");

	// 사용 가능한 스킬들을 전부 추가한다
	TArray<FCurrentSkillInfo> UsableSkill;
	
	// 모든 스킬에 대하여 순회
	for(auto MonsterSkill : Monster->GetMonsterCombatComponent()->GetMonsterSkills())
	{
		if(MonsterSkill.GetSkill()->GetSkillDistance() >= CurrentDistance) // 타겟에 대해 해당 스킬을 사용할 거리가 되는가?
		{
			float TimeRemaining, CooldownDuration;
			// 태그 및 쿨타임 확인
			bool bCheck = Monster->GetCooldownRemainingForTag(*MonsterSkill.GetSkill()->GetCooldownTags(), TimeRemaining, CooldownDuration);
			if(bCheck)
			{
				if(TimeRemaining <= 0.f)
				{
					UsableSkill.Add(MonsterSkill);
				}
			}
			else
			{
				UsableSkill.Add(MonsterSkill);
			}
			
		}
	}

	// 사용 가능한 스킬 중 하나 랜덤으로 Set
	if(UsableSkill.Num() > 0)
	{
		UMonsterSkillBase* SelectedSkill = GetRandomMonsterSkillWithWeight(UsableSkill);
		MY_LOG(LogTemp, Warning, TEXT("Skill Set : %s"), *SelectedSkill->SkillName);
	}
}

UMonsterSkillBase* UBTService_SetSkill::GetRandomMonsterSkillWithWeight(const TArray<FCurrentSkillInfo>& UsableSkills)
{
	TArray<float> Values;
	Values.Reserve(20); // 미리 20의 공간 할당

	float sum = 0.f;
	for(int i = 0; i < UsableSkills.Num(); i++)
	{
		sum += UsableSkills[i].GetSkillWeight();
		Values.Add(UsableSkills[i].GetSkillWeight());
	}
	if(sum == 0.f) return nullptr;

	float RandFloat = FMath::RandRange(0.f, sum);
	for(int i = 0; i < UsableSkills.Num(); i++)
	{
		if(Values[i] > 0)
		{
			RandFloat -= Values[i];
			if(RandFloat <= 0)
			{
				return UsableSkills[i].GetSkill();
			}
		}
	}
	return nullptr;
}

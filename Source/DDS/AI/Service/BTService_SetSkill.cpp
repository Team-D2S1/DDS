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
	TArray<UMonsterSkillBase*> UsableSkill;
	
	// 모든 스킬에 대하여 순회
	for(auto MonsterSkill : Monster->GetMonsterCombatComponent()->GetMonsterSkills())
	{
		if(MonsterSkill->GetSkillDistance() >= CurrentDistance) // 거리가 되는지
		{
			float TimeRemaining, CooldownDuration;
			bool bCheck = Monster->GetCooldownRemainingForTag(*MonsterSkill->GetCooldownTags(), TimeRemaining, CooldownDuration);
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
		int32 RandNum = FMath::RandRange(0, UsableSkill.Num() - 1);
		Blackboard->SetValueAsObject("SelectedSkill", UsableSkill[RandNum]);
		MY_LOG(LogTemp, Warning, TEXT("Skill Set : %s"), *UsableSkill[RandNum]->SkillName);
	}
}

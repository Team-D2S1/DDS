// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Task/BTTask_NormalAttack.h"

#include "AbilitySystemComponent.h"
#include "AI/AIController/AIControllerBase.h"
#include "AI/Skills/MonsterSkillBase.h"
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

	CachedOwnerComp = &OwnerComp;
	
	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!Monster) return EBTNodeResult::Failed;

	UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	// 발동할 스킬 가져오기
	UGameplayAbility* CurrentSkill = Cast<UGameplayAbility>(Blackboard->GetValueAsObject("SelectedSkill"));
	if(!CurrentSkill) return EBTNodeResult::Failed;

	// AbilitySpec 가져오기 (생성은 MonsterCombatComponent에서)
	FGameplayAbilitySpec* FoundSpec = ASC->FindAbilitySpecFromClass(CurrentSkill->GetClass());
	if(!FoundSpec) return EBTNodeResult::Failed;
	
	// 스킬 종료 콜백 바인딩 (스킬 종료는 보통 몽타주에서 불린다)
	OnAbilityEndDelegateHandle = FoundSpec->Ability->OnGameplayAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);

	// 스킬 성공했으면 InProgress, 실패했으면 Failed
	const bool bStarted = ASC->TryActivateAbility(FoundSpec->Handle);
	return bStarted ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

void UBTTask_NormalAttack::OnAbilityEnded(UGameplayAbility* Ability)
{
	Ability->OnGameplayAbilityEnded.Remove(OnAbilityEndDelegateHandle);

	if(CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

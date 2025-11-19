// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Task/BTTaskNode_ActivateSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/GameplayAbility.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "ETC/CustomLog.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTaskNode_ActivateSkill::UBTTaskNode_ActivateSkill()
{
	NodeName = "Activate Skill";
	bNotifyTick = true;
	
	/* UBTTaskNode는 기본적으로 클래스 단위로 하나의 인스턴스만 생성된다
	 * 즉, 동일한 Task를 여러 AI가 공유하면 모든 AI가 동일한 Task 인스턴스를 공유하게 됨
	 * AI마다 다른 값을 가지는 Task를 만들고 싶다면 bCreateNodeInstance를 true로 해야 한다
	 * 내부 설명 : if set, node will be instanced instead of using memory block and template shared with all other BT components
	 */
	bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTTaskNode_ActivateSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	// Initialize
	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	CachedOwnerComp = &OwnerComp;

	// Execute Condition Check
	if(Blackboard->GetValueAsBool("bIsUsingSkill")) return EBTNodeResult::Failed; 
	
	// 발동할 스킬 가져오기
	UGameplayAbility* CurrentSkill = Cast<UGameplayAbility>(Blackboard->GetValueAsObject("SelectedSkill"));
	if(!CurrentSkill) return EBTNodeResult::Failed;
	
	// 스킬에 대한 AbilitySpec은 이미 MonsterCombatComponent에서 만들어져 있다
	// 스킬을 발동시키기 위한 AbilitySpec을 캐릭터의 ASC에서 가져온다
	FGameplayAbilitySpec* FoundSpec = ASC->FindAbilitySpecFromClass(CurrentSkill->GetClass());
	CachedHandle = FoundSpec->Handle;
	if(!FoundSpec) return EBTNodeResult::Failed;
	
	// 스킬 사용에 성공했다면 InProgress로 노드 대기, 실패했다면 Failed 반환
	const bool bSuccessful = ASC->TryActivateAbility(FoundSpec->Handle);
	if(bSuccessful)
	{
		MY_LOG(LogTemp, Warning, TEXT("Skill Activated Successfully!"));
		Blackboard->SetValueAsBool("bIsUsingSkill", true);
		// 스킬 종료시 불릴 콜백함수 바인딩

		// 점프하는 RootMotion이면 FlyMode 켜준다
		if(CurrentSkill->IsA(UMonsterSkillBase::StaticClass()))
		{
			if(UMonsterSkillBase* MonsterSkill = Cast<UMonsterSkillBase>(CurrentSkill))
			{
				if(MonsterSkill->GetIsJumpSKill())
				{
					UCharacterMovementComponent* MovementComponent = Monster->GetCharacterMovement();
					MovementComponent->SetMovementMode(MOVE_Flying);
				}
			}
		}
		
		OnAbilityEndDelegateHandle = ASC->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEnded);

		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTTaskNode_ActivateSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor")))
	{
		AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

		FVector TargetVector = TargetActor->GetActorLocation() - Monster->GetActorLocation();
		FRotator TargetRotation = TargetVector.Rotation();
		TargetRotation.Pitch = 0.f; TargetRotation.Roll = 0.f;

		FRotator CurrentRotation = Monster->GetActorRotation();

		float RotationSpeed = 360.f;

		FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);

		Monster->SetActorRotation(NewRotation);
	}
}

void UBTTaskNode_ActivateSkill::OnAbilityEnded(const FAbilityEndedData& EndedData)
{
	if(EndedData.AbilitySpecHandle == CachedHandle)
	{
		if(CachedOwnerComp)
		{
			if(AMonsterBase* Monster = Cast<AMonsterBase>(CachedOwnerComp->GetAIOwner()->GetPawn()))
			{
				UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
				ASC->OnAbilityEnded.Remove(OnAbilityEndDelegateHandle);

				// FlyMode 켜져있다면 꺼준다
				if(UCharacterMovementComponent* MovementComponent = Monster->GetCharacterMovement())
				{
					if(MovementComponent->MovementMode == MOVE_Flying)
					{
						MovementComponent->SetMovementMode(MOVE_Walking);
					}
				}
			}
			
			CachedOwnerComp->GetBlackboardComponent()->SetValueAsBool("bIsUsingSkill", false);
			CachedOwnerComp->GetBlackboardComponent()->SetValueAsObject("SelectedSkill", nullptr);

			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
			CachedOwnerComp = nullptr;
		}
	}
	
}

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Skills/MonsterSkillBase.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Monster/MonsterBase.h"
#include "ETC/CustomLog.h"

UMonsterSkillBase::UMonsterSkillBase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

bool UMonsterSkillBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                           const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UMonsterSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMonsterBase* Monster = Cast<AMonsterBase>(ActorInfo->AvatarActor);
	if(!Monster) return;

	// 몬스터 방향 돌리기
	if(UBlackboardComponent* Blackboard = Cast<AAIController>(Monster->GetController())->GetBlackboardComponent())
	{
		const AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));

		if(Target)
		{
			FRotator MonsterRotation = (Target->GetActorLocation() - Monster->GetActorLocation()).Rotation();
			MonsterRotation.Pitch = 0.f; MonsterRotation.Roll = 0.f;
			Monster->SetActorRotation(MonsterRotation);	
		}
	}
	
	// 몽타주 실행
	if(SkillMontage)
	{
		auto* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			SkillMontage,
			1.f,
			NAME_None,
			true);
		Task->OnCompleted.AddDynamic(this, &ThisClass::OnSkillMontageEnded);
		Task->OnInterrupted.AddDynamic(this, &ThisClass::OnSkillMontageInterrupted);
		Task->ReadyForActivation();
	}
	
	for(auto Effect : SkillEffects)
	{
		BP_ApplyGameplayEffectToOwner(Effect);
	}
	
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
}

bool UMonsterSkillBase::CommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if(Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags) == false) return false;
	
	return true;
}

void UMonsterSkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	bool bIsSuccess = TryActivateNextSkill();
}

void UMonsterSkillBase::OnSkillMontageEnded()
{
	if(IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMonsterSkillBase::OnSkillMontageInterrupted()
{
	if(IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

bool UMonsterSkillBase::TryActivateNextSkill()
{
	if(!NextSkill) return false;
	
	AMonsterBase* Monster = Cast<AMonsterBase>(CurrentActorInfo->AvatarActor);
	if(!Monster) return false;

	UBlackboardComponent* Blackboard;
	if(Monster->GetController())
	{
		 Blackboard = Cast<AAIController>(Monster->GetController())->GetBlackboardComponent();
	}else return false;


	const AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
	if(!Target) return false;

	const FGameplayAbilitySpec* FoundSpec = Monster->GetAbilitySystemComponent()->FindAbilitySpecFromClass(NextSkill);
	if(!FoundSpec) return false;
	
	// 거리가 스킬 사정거리보다 가까울 경우에만 스킬 실행
	FVector ToTarget = Target->GetActorLocation() - Monster->GetActorLocation();
	if(ToTarget.Length() <= Cast<UMonsterSkillBase>(FoundSpec->Ability)->SkillDistance)
	{
		Blackboard->SetValueAsObject("SelectedSkill", FoundSpec->Ability);
		return true;
		//return Monster->GetAbilitySystemComponent()->TryActivateAbility(FoundSpec->Handle);
	}
	
	return false;
}

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Skills/MonsterSkillBase.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
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
	
	// 몽타주 실행
	if(!SkillMontage) return;

	const AMonsterBase* Monster = Cast<AMonsterBase>(ActorInfo->AvatarActor);
	if(Monster)
	{
		if(UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
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

	if(NextSkill)
	{
		// TODO!
		// 조건 체크 : 플레이어가 다음 스킬의 사정거리 안에 있을 것
		// 행동 변화 : 플레이어를 향해 방향을 돌릴 것
		
		if(AMonsterBase* Monster = Cast<AMonsterBase>(CurrentActorInfo->AvatarActor))
		{
			FGameplayAbilitySpec* FoundSpec = Monster->GetAbilitySystemComponent()->FindAbilitySpecFromClass(NextSkill->GetClass());
			if(FoundSpec)
			{
				const bool bSuccessful = Monster->GetAbilitySystemComponent()->TryActivateAbility(FoundSpec->Handle);
				if(bSuccessful) return;
			}
		}
	}
	else
	{
		OnGameplayAbilityEnded.Broadcast(this);
		return;
	}
	
	OnGameplayAbilityEnded.Broadcast(this);
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

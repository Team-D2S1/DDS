// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "AI/Skills/MonsterSkillBase.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
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
	
	// 몽타주 실행
	if(!SkillMontage) return;

	const AMonsterBase* Monster = Cast<AMonsterBase>(ActorInfo->AvatarActor);
	if(Monster)
	{
		if(UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(SkillMontage);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UMonsterSkillBase::OnSkillMontageEnded);

			AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillMontage);
		}
	}
}

bool UMonsterSkillBase::CommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	if(Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags) == false) return false;

	MY_LOG(LogTemp, Log, TEXT("Commit Ability"));

	return true;
}

void UMonsterSkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	OnGameplayAbilityEnded.Broadcast(this);
}

void UMonsterSkillBase::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "AI/Skills/MonsterSkillBase.h"
#include "Character/Monster/MonsterBase.h"



UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::ApplyDamageToTarget(APlayerBase* Player)
{
	if(!GetOwner()->HasAuthority()) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetOwner());

	bool bPlayerDodge = false;
	bPlayerDodge = Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(DDSGameplayTags::Player_State_Dodging);

	if(bPlayerDodge) return;
	
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(HitEffect, 1.0f, EffectContext);

	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), Player->GetAbilitySystemComponent());
}

void UMonsterCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(!GetOwner()->HasAuthority()) return;

	AMonsterBase* Monster = Cast<AMonsterBase>(GetOwner());
	for(auto& SkillInfo : MonsterSkillClassInfos)
	{
		if(SkillInfo.MonsterSkillClass)
		{
			FGameplayAbilitySpec Spec(SkillInfo.MonsterSkillClass, 1, INDEX_NONE);
			Monster->GetAbilitySystemComponent()->GiveAbility(Spec);

			if(SkillInfo.bIsUsable)
			{
				if(UMonsterSkillBase* MonsterSkill = Cast<UMonsterSkillBase>(Spec.Ability))
				{
					MonsterSkills.Add(FCurrentSkillInfo(MonsterSkill, SkillInfo.BaseSkillWeight));
				}
			}
		}
	}
}

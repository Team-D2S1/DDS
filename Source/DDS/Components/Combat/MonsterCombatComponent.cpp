// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "Character/Monster/MonsterBase.h"
#include "ETC/CustomLog.h"


UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::Attack()
{
	if(GetOwnerRole() == ROLE_Authority)
	{
		NetMulticast_Attack();
	}
}

void UMonsterCombatComponent::NetMulticast_Attack_Implementation()
{
	AMonsterBase* OwningMonster = Cast<AMonsterBase>(GetOwner());
	UAnimInstance* AnimInstance = OwningMonster->GetMesh()->GetAnimInstance();
	if(AnimInstance && NormalAttackMontage)
	{
		AnimInstance->Montage_Play(NormalAttackMontage);
	}
}

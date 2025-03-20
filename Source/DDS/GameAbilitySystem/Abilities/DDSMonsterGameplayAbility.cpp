// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameAbilitySystem/Abilities/DDSMonsterGameplayAbility.h"

#include "Character/Monster/MonsterBase.h"


AMonsterBase* UDDSMonsterGameplayAbility::GetMonsterCharacterFromActorInfo()
{
	if (!CachedMonsterCharacter.IsValid())
	{
		CachedMonsterCharacter = Cast<AMonsterBase>(CurrentActorInfo->AvatarActor);
	}
	return CachedMonsterCharacter.IsValid() ? CachedMonsterCharacter.Get() : nullptr;
}

UMonsterCombatComponent* UDDSMonsterGameplayAbility::GetMonsterCombatComponentFromActorInfo()
{
	return GetMonsterCharacterFromActorInfo()->GetMonsterCombatComponent();
}

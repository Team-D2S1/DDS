// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/MonsterCombatComponent.h"

#include "ETC/CustomLog.h"


UMonsterCombatComponent::UMonsterCombatComponent()
{
	
}

void UMonsterCombatComponent::Attack()
{
	MY_LOG(LogTemp, Error, TEXT("공격!"));
}

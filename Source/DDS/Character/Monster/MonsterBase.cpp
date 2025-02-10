// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MonsterBase.h"

#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"


AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UDDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);//
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDDSAttributeSet>(TEXT("AttributeSet"));
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
		return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

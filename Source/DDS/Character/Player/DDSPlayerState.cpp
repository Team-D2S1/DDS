// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSPlayerState.h"

#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"


ADDSPlayerState::ADDSPlayerState()
{
	// 서버 클라이언트 동기화 주기 설정
	SetNetUpdateFrequency(100.f);


	AbilitySystemComponent = CreateDefaultSubobject<UDDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);//
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDDSAttributeSet>(TEXT("AttributeSet"));
}

// void ADDSPlayerState::PrintAllAbilities() const
// {	
// 	if (AbilitySystemComponent)
// 	{
// 		for (const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
// 		{
// 			UE_LOG(LogTemp, Log, TEXT("Ability: %s, Level: %d"),
// 				*AbilitySpec.Ability->GetClass()->GetName(), 
// 				AbilitySpec.Level)s
// 		}
// 	}
// }

UAbilitySystemComponent* ADDSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UDDSAbilitySystemComponent* ADDSPlayerState::GetDDSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADDSPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

UDDSAttributeSet* ADDSPlayerState::GetDDSAttribueSet() const
{
	return AttributeSet;
}


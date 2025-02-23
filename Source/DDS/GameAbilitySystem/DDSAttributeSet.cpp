// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UDDSAttributeSet::UDDSAttributeSet()
{
}

void UDDSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 각 GAD를 Replicated 되도록 등록
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}


/**
 * @brief 데이터가 서버에서 클라이언트로 전달될 때 호출됨.
 */
void UDDSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// AbilitySystem에게 알려주기
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Health, OldHealth);
}

void UDDSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MaxHealth, OldMaxHealth);
}

void UDDSAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Stamina, OldStamina);
}

void UDDSAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MaxStamina, OldMaxStamina);
}
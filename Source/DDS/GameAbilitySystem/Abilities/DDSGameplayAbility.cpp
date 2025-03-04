// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "ETC/CustomLog.h"

void UDDSGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!ActorInfo->AvatarActor->HasAuthority())
	{
		return;
	}
	
	MY_LOG(LogTemp,Log,TEXT("OnGiveAbility"))
	if (AbilityActivationPolicy == EDDSAbilityActivationPolicy::OnGiven)
	{
		MY_LOG(LogTemp,Log,TEXT("OnGiven Policy"))
		if (ActorInfo)// && Spec.IsActive())
		{
			MY_LOG(LogTemp,Log,TEXT("TryActivateAbility %s"),*Spec.Ability->GetName())
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UDDSGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityActivationPolicy == EDDSAbilityActivationPolicy::OnGiven && HasAuthority(&ActivationInfo))
	{
		if (ActorInfo)
		{
			MY_LOG(LogTemp,Log,TEXT("ClearAbility %s"),*GetName())
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UDDSGameplayAbility::GetPawnCombatComponent() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAbilitySystemComponent.h"

#include "Abilities/DDSGameplayAbility.h"
#include "ETC/CustomLog.h"

void UDDSAbilitySystemComponent::AbilityActorInfoSet()
{
	// Effect 적용시 서버에서 호출되는 이벤트 델리게이트
	// OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDDSAbilitySystemComponent::OnGameplayEffectAppliedToSelf);
}

void UDDSAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UDDSGameplayAbility>& Ability : StartupAbilities)
	{
		if (!Ability)
			continue;
		
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = 1;
		MY_LOG(LogTemp,Type::Log,"Granting %s",*Ability->GetName());
		// TSubclassOf에서 UDDSGameplayAbility 가져오기
		// UDDSGameplayAbility* UDDSGameplayAbility = Ability.GetDefaultObject();
		// // Ongiven처리
		// if (UDDSGameplayAbility->GetAbilityActivationPolicy() == EDDSAbilityActivationPolicy::OnGiven)
		// {
		// 	
		// 	GiveAbilityAndActivateOnce(AbilitySpec);
		// }else
		{
			GiveAbility(AbilitySpec);
		}

	}
}

void UDDSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
}


void UDDSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

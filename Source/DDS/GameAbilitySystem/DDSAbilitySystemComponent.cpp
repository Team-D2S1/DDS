// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAbilitySystemComponent.h"

#include "Abilities/DDSGameplayAbility.h"
#include "DDSTypes/DDSStructTypes.h"
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
	if (!InputTag.IsValid())
	{
		return;
	}
	for (const FGameplayAbilitySpec& spec : GetActivatableAbilities())
	{
		if (spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			// spec 이름 출력
			bool isServer = GetOwner()->HasAuthority();
			DEBUG_CLOG_DISPLAY_NET(FColor::Emerald,isServer,TEXT("Try to Activate Ability : %s "),*spec.Ability->GetName());
			TryActivateAbility(spec.Handle);
		}
	}
}


void UDDSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

void UDDSAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FDDSPlayerAbilitySet>& InDefaultWeaponAbilities,
	int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		DEBUG_LOG_DISPLAY("InDefaultWeaponAbilities is Empty");
		return;
	}
	for (const FDDSPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
			continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		AbilitySpec.Level = ApplyLevel;
		OutHandles.AddUnique( GiveAbility(AbilitySpec) );
		// MY_LOG(LogTemp, Log, TEXT("Granting %s, tag: %s"), *AbilitySet.AbilityToGrant->GetName(), *AbilitySet.InputTag.ToString());
		bool isServer = GetOwner()->HasAuthority();
		DEBUG_LOG_DISPLAY_NET(isServer, TEXT("Granting %s, tag: %s"), *AbilitySet.AbilityToGrant->GetName(), *AbilitySet.InputTag.ToString());
	}
}

void UDDSAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InHandles)
{
	for (const FGameplayAbilitySpecHandle& Handle : InHandles)
		{
			if (Handle.IsValid())
			{
				ClearAbility(Handle);
			}
		}
	InHandles.Empty();
}

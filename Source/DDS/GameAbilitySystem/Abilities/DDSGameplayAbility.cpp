// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

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

UPawnCombatComponent* UDDSGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UDDSAbilitySystemComponent* UDDSGameplayAbility::GetDDSAbilitySystemComponentFromActorInfo() const
{
	return Cast<UDDSAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UDDSGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InEffectSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC)
	{
		MY_ERROR_DISPLAY_NET(CurrentActorInfo->OwnerActor->HasAuthority(), TEXT("TargetASC is nullptr"));
		return FActiveGameplayEffectHandle();
	}
	if (!InEffectSpecHandle.IsValid())
	{
		MY_ERROR_DISPLAY_NET(CurrentActorInfo->OwnerActor->HasAuthority(), TEXT("InEffectSpecHandle is invalid"));
		return FActiveGameplayEffectHandle();
	}
	return GetDDSAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget( 
		*InEffectSpecHandle.Data,
		TargetASC
	);
}

FActiveGameplayEffectHandle UDDSGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InEffectSpecHandle, EDDSSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle Handle = NativeApplyEffectSpecHandleToTarget(TargetActor, InEffectSpecHandle);
	if (Handle.IsValid())
	{
		OutSuccessType = EDDSSuccessType::Success;
	}
	else
	{
		OutSuccessType = EDDSSuccessType::Fail;
	}
	return Handle;
}



FGameplayEffectSpecHandle UDDSGameplayAbility::MakeGameplayEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,
                                                                            float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentAttackComboCount) const
{
	if (!EffectClass)
	{
		MY_ERROR_DISPLAY_NET(CurrentActorInfo->OwnerActor->HasAuthority(), TEXT("EffectClass is nullptr"));
		return FGameplayEffectSpecHandle();
	}

	FGameplayEffectContextHandle ContextHandle = GetDDSAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	
	FGameplayEffectSpecHandle EffectSpecHandle = GetDDSAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(DDSGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InCurrentAttackComboCount);
	}
	return EffectSpecHandle;
}

// void UDDSGameplayAbility::PlayMontageAndWaitForEvent(UAnimMontage* Montage, FName SectionName, FGameplayTag EventTag,
// 	float Rate, FName StartSection)
// {
// 	// if (CurrentActorInfo->OwnerActor->HasAuthority() && Montage)
// 	// {
// 	// 	UAbilityTask *Task =
// 	// }
// }
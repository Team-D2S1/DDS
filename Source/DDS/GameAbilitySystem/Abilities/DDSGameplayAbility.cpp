// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "GameAbilitySystem/Abilities/DDSGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "Character/EntityBase.h"
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
	
	// MY_LOG(LogTemp,Log,TEXT("OnGiveAbility"))
	if (AbilityActivationPolicy == EDDSAbilityActivationPolicy::OnGiven)
	{
		// MY_LOG(LogTemp,Log,TEXT("OnGiven Policy"))
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

FVector UDDSGameplayAbility::GetCachedDodgeInputDirection() const
{
	UDDSAbilitySystemComponent* ASC = GetDDSAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		return FVector::ForwardVector;
	}
	
	// ASC에 저장된 것은 이미 월드 좌표
	FVector WorldDirection = ASC->GetLastDodgeInputDirection();
	
	// #if WITH_EDITOR
	// AEntityBase* Entity = Cast<AEntityBase>(GetAvatarActorFromActorInfo());
	// bool bIsServer = Entity && Entity->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Yellow, bIsServer, TEXT("[GA] GetCachedDodgeInputDirection: World=%s"), *WorldDirection.ToString());
	// #endif
	
	return WorldDirection.GetSafeNormal();
}

EMoveDirection4 UDDSGameplayAbility::GetMoveDirection4FromWorld(const FVector& WorldDirection) const
{
	AEntityBase* Entity = Cast<AEntityBase>(GetAvatarActorFromActorInfo());
	if (!Entity)
	{
		return EMoveDirection4::None;
	}
	
	EMoveDirection4 Direction = Entity->GetMoveDirection4(WorldDirection);
	
	// #if WITH_EDITOR
	// bool bIsServer = Entity->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Green, bIsServer, TEXT("[GA] GetMoveDirection4: World=%s, Result=%d"), *WorldDirection.ToString(), (int32)Direction);
	// #endif
	
	return Direction;
}

EMoveDirection8 UDDSGameplayAbility::GetMoveDirection8FromWorld(const FVector& WorldDirection) const
{
	AEntityBase* Entity = Cast<AEntityBase>(GetAvatarActorFromActorInfo());
	if (!Entity)
	{
		return EMoveDirection8::None;
	}
	
	return Entity->GetMoveDirection8(WorldDirection);
}

// void UDDSGameplayAbility::PlayMontageAndWaitForEvent(UAnimMontage* Montage, FName SectionName, FGameplayTag EventTag,
// 	float Rate, FName StartSection)
// {
// 	// if (CurrentActorInfo->OwnerActor->HasAuthority() && Montage)
// 	// {
// 	// 	UAbilityTask *Task =
// 	// }
// }
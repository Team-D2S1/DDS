// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DDSGameplayTags.h"
#include "Character/EntityBase.h"
#include "Components/BoxComponent.h"
#include "ETC/CustomLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/DDSWeaponBase.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, ADDSWeaponBase* InWeapon,
                                                 bool bRegisterAsEquippedWeapon)
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		MY_LOG(LogTemp, Error, TEXT("Weapon Tag %s is already registered."), *InWeaponTag.ToString());
		return;
	}

	CharacterCarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	// UFunction() 이면 DynamicDelegate로 해야함
	InWeapon->OnWeaponHitTarget.BindUObject(this, &UPawnCombatComponent::OnHitTarget);
	InWeapon->OnWeaponPulledFromTarget.BindUObject(this, &UPawnCombatComponent::OnPulledFromTarget);
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTag;
	}

	bool hasAuthority = GetOwningPawn()->HasAuthority();
	if (hasAuthority)
	{
		//클라이언트도 RegisterSpawnedWeapon을 호출하게 하기 위해
		InWeapon->SetOwnerPawn(GetOwningPawn());
		InWeapon->SetWeaponTag(InWeaponTag);
	}
	DEBUG_LOG_DISPLAY_NET(hasAuthority, TEXT("A Weapon %s (Tag: %s) is registered."), *InWeapon->GetName(), *InWeaponTag.ToString());
}



void UPawnCombatComponent::ToggleWeaponCollision(bool bEnable, EToggleCollisionType InDamageType)
{
	if (InDamageType == EToggleCollisionType::CurrentEquippedWeapon)
	{
		ADDSWeaponBase* CurrentWeapon = GetCurrentEquippedWeapon();
		if (CurrentWeapon)
		{
			if (bEnable)
			{
				CurrentWeapon->GetWeaponCollsionBox()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
				// DEBUG_CLOG_DISPLAY_NET(FColor::Green, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is enabled."));
			}
			else
			{
				CurrentWeapon->GetWeaponCollsionBox()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
				OverlappedActors.Empty();
				// DEBUG_CLOG_DISPLAY_NET(FColor::Red, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is not disabled."));
			}
			
		}else
		{
			DEBUG_CLOG_DISPLAY_NET(FColor::Red, GetOwningPawn()->HasAuthority(), TEXT("Current Weapon is not equipped."));
		}
	}
	//TODO : 무기가 없는 경우 처리
}

void UPawnCombatComponent::OnHitTarget(AActor* InTargetActor)
{
	if (OverlappedActors.Contains(InTargetActor))
	{
		return;
	}
	OverlappedActors.Add(InTargetActor); // 어차피 위에서 확인함

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = InTargetActor;
	// TODO : 무기 정보도 넘기기

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		DDSGameplayTags::Shared_Event_MeleeHit_Start,
		EventData);
}

void UPawnCombatComponent::OnPulledFromTarget(AActor* InTargetActor)
{
	if (!OverlappedActors.Contains(InTargetActor))
	{
		return;
	}
	OverlappedActors.Remove(InTargetActor);
	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = InTargetActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		DDSGameplayTags::Shared_Event_MeleeHit_End,
		EventData);
}

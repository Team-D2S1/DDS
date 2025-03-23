// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PawnCombatComponent.h"

#include "Character/EntityBase.h"
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

ADDSWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeapon(FGameplayTag InWeaponTag) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTag))
	{
		if (ADDSWeaponBase* const* res = CharacterCarriedWeaponMap.Find(InWeaponTag))
		{
			return *res;
		}
	}
	MY_LOG(LogTemp, Error, TEXT("Weapon Tag %s is not registered."), *InWeaponTag.ToString());
	return nullptr;
}

ADDSWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeapon(CurrentEquippedWeaponTag);
}




// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items/Weapons/DDSPlayerWeapon.h"

ADDSPlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADDSPlayerWeapon>(GetCharacterCarriedWeapon(InWeaponTag));
}

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace DDSGameplayTags
{
	/** Input Tags */
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LockOn)

	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipWeapon01)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipWeapon01)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipWeapon02)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipWeapon02)

	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack)

	/** Player Tags */
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_LockOn)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Sword)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Sword)
	
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_LightAttack_Sword)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_HeavyAttack_Sword)
	
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Sword)
	
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Sword)
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Sword)

	/** Monster Tags */
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Monster_Weapon)

	/** Shared Tags */

	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);

	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);
	
	DDS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_State_LockedOn)
	
}
 
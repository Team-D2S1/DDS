// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSGameplayTags.h"

namespace DDSGameplayTags
{
	/** Input Tags */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "Input.Move", "Input : moving the player character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "Input.Look", "Input : looking around player character.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "Input.Jump", "Input : making the player character jump.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LockOn, "Input.LockOn", "Input : locking onto a target.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint, "Input.Sprint", "Input : sprinting.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Walk, "Input.Walk", "Input : walking.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "Input.Crouch", "Input : crouching.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UI_Escape, "Input.UI.Escape", "Input : pressing the escape key in the UI.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UI_Crafting_ToggleUI, "Input.UI.Crafting.ToggleUI", "Input : toggling the crafting UI.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UI_Crafting_RemoveItem, "Input.UI.Crafting.RemoveItem","Input : remove current item");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipWeapon01, "Input.Equip.Weapon01", "Input : equipping the player's first weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnequipWeapon01, "Input.Unequip.Weapon01", "Input : unequipping the player's first weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_EquipWeapon02, "Input.Equip.Weapon02", "Input : equipping the player's second weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UnequipWeapon02, "Input.Unequip.Weapon02", "Input : unequipping the player's second weapon.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LightAttack, "Input.LightAttack", "Input : performing a light attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_HeavyAttack, "Input.HeavyAttack", "Input : performing a heavy attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_DodgeB, "Input.DodgeB", "Input : B button for dodge/backstep/sprint modifier.");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Debug_PrintAttributes, "Input.Debug.PrintAttributes");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseHealthPotion, "Input.UseHealthPotion", "Input : using a health potion.");

	// Cheat Input Tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Cheat_AddExp, "Input.Cheat.AddExp");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Cheat_AddAttributePoints, "Input.Cheat.AddAttributePoints");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Cheat_LevelUp, "Input.Cheat.LevelUp");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Cheat_FullHeal, "Input.Cheat.FullHeal");
	/** UI Widget Tags */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(WidgetTag_Crafting, "Widget.Crafting", "Widget : crafting widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_MainMenu, "UI.Widget.MainMenu", "UI Widget : main menu widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Equip, "UI.Widget.Equip", "UI Widget : equip widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Weapon, "UI.Widget.Weapon", "UI Widget : weapon widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Armor, "UI.Widget.Armor", "UI Widget : armor widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_WeaponCraft, "UI.Widget.WeaponCraft", "UI Widget : weapon craft widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Crafting, "UI.Widget.Crafting", "UI Widget : crafting widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Item, "UI.Widget.Item", "UI Widget : item widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_Status, "UI.Widget.Status", "UI Widget : status widget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(UI_Widget_System, "UI.Widget.System", "UI Widget : system widget.");

	/** Attribute Tags */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Default_Level, "Attribute.Default.Level", "Attribute : default level.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Default_Energy, "Attribute.Default.Energy", "Attribute : default energy.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Default_RequireEnergy, "Attribute.Default.RequireEnergy", "Attribute : default require energy.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Default_Soul, "Attribute.Default.Soul", "Attribute : default souls.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Default_AttributePoints, "Attribute.Default.AttributePoints", "Attribute : default attribute points.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Vitality, "Attribute.Primary.Vitality", "Attribute : primary vitality.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Endurance, "Attribute.Primary.Endurance", "Attribute : primary endurance.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Strength, "Attribute.Primary.Strength", "Attribute : primary strength.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Dexterity, "Attribute.Primary.Dexterity", "Attribute : primary dexterity.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Magic, "Attribute.Primary.Magic", "Attribute : primary magic.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_PlayerStats, "GameplayEffect.PlayerStats", "GameplayEffect : player stats effect tag for refreshing MMC calculations.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_Health, "Attribute.Status.Health", "Attribute : status health.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_Stamina, "Attribute.Status.Stamina", "Attribute : status stamina.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_Mana, "Attribute.Status.Mana", "Attribute : status mana.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_HealthMax, "Attribute.Status.HealthMax", "Attribute : status health max.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_StaminaMax, "Attribute.Status.StaminaMax", "Attribute : status stamina max.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Status_ManaMax, "Attribute.Status.ManaMax", "Attribute : status mana max.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_BaseAttack, "Attribute.Offense.BaseAttack", "Attribute : offense base attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_BaseAttackPlus, "Attribute.Offense.BaseAttackPlus", "Attribute : offense base attack plus.");

	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_StrengthAR, "Attribute.Offense.StrengthAR");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_StrengthARPlus, "Attribute.Offense.StrengthARPlus");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_DexterityAR, "Attribute.Offense.DexterityAR");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_DexterityARPlus, "Attribute.Offense.DexterityARPlus");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_MagicAR, "Attribute.Offense.MagicAR");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Offense_MagicARPlus, "Attribute.Offense.MagicARPlus");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_PhysicalASR, "Attribute.Offense.PhysicalASR", "Attribute : offense physical attack skill ratio.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_DexterityASR, "Attribute.Offense.DexterityASR", "Attribute : offense dexterity attack skill ratio.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_MagicASR, "Attribute.Offense.MagicASR", "Attribute : offense magic attack skill ratio.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Weapon_AttackRequireStamina, "Attribute.Weapon.AttackRequireStamina", "Attribute : weapon attack stamina requirement.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_PhysicalPower, "Attribute.Offense.PhysicalPower", "Attribute : offense physical power.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Offense_MagicPower, "Attribute.Offense.MagicPower", "Attribute : offense magic power.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Defense_PhysicalDefense, "Attribute.Defense.PhysicalDefense", "Attribute : defense physical defense.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Defense_MagicDefense, "Attribute.Defense.MagicDefense", "Attribute : defense magic defense.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Defense_PhysicalResist, "Attribute.Defense.PhysicalResist", "Attribute : defense physical resist.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Defense_MagicResist, "Attribute.Defense.MagicResist", "Attribute : defense magic resist.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Item_HealthPotion, "Attribute.Item.HealthPotion", "Attribute : Health Potion Count");

	/** Player Tags */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Move, "Player.Ability.Move", "Player : player moves.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Jump, "Player.Ability.Jump", "Player : player jumps.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_LockOn, "Player.Ability.LockOn", "Player : player locks onto a target.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword", "Player : player equips a sword ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Unequip_Sword, "Player.Ability.Unequip.Sword", "Player : player unequips a sword ability.");


	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Equip_Crafted, "Player.Ability.Equip.Crafted", "Player : player equips a crafted weapon ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Unequip_Crafted, "Player.Ability.Unequip.Crafted", "Player : player unequips a crafted weapon ability.");


	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_LightAttack_Sword, "Player.Ability.Attack.LightAttack.Sword", "Player : player performs a light attack ability with a sword.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Attack_HeavyAttack_Sword, "Player.Ability.Attack.HeavyAttack.Sword", "Player : player performs a heavy attack ability with a sword.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Dodge, "Player.Ability.Dodge", "Player : player performs a dodge ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_Backstep, "Player.Ability.Backstep", "Player : player performs a backstep ability.");
   	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Ability_UseHealthPotion, "Player.Ability.UseHealthPotion", "Player : player uses a health potion ability.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Sword, "Player.Weapon.Sword", "Player : player's sword weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Weapon_Crafted, "Player.Weapon.Crafted", "Player : player's crafted weapon.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_StopStaminaRegen, "Player.State.StopStaminaRegen", "Player : player's stamina regeneration is stopped.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_Dodging, "Player.State.Dodging", "Player : player is dodging.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_BlockMove, "Player.State.BlockMove", "Player : player's movement is blocked.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_Moving, "Player.State.Moving", "Player : player is moving.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_Sprinting, "Player.State.Sprinting", "Player : player is sprinting.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_State_Walking, "Player.State.Walking", "Player : player is walking slowly.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equip_Sword, "Player.Event.Equip.Sword", "Player : player equips a sword.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Unequip_Sword, "Player.Event.Unequip.Sword", "Player : player unequips a sword.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Equip_Crafted, "Player.Event.Equip.Crafted", "Player : player equips a crafted weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Unequip_Crafted, "Player.Event.Unequip.Crafted", "Player : player unequips a crafted weapon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_Event_Death, "Player.Event.Death", "Player : player has died.");


	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light", "Player : light attack type set by caller.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy", "Player : heavy attack type set by caller.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_AddEnergy, "Player.SetByCaller.AddEnergy", "Player : add energy set by caller.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Player_SetByCaller_ConsumeStamina, "Player.SetByCaller.ConsumeStamina", "Player : consume stamina set by caller.");
	
	// Weapon SetByCaller Tags (Blade / Grip stats)
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_Level, "Weapon.SetByCaller.Level", "Weapon : level (level).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_BaseATK, "Weapon.SetByCaller.BaseATK", "Weapon : blade base attack (BaseATK).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_BaseATKPlus, "Weapon.SetByCaller.BaseATKPlus", "Weapon : blade base attack plus (BaseATKplus).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_PowASR, "Weapon.SetByCaller.PowASR", "Weapon : blade strength scaling (powASR).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_PowASRPlus, "Weapon.SetByCaller.PowASRPlus", "Weapon : blade strength scaling plus (powASRplus).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_DexASR, "Weapon.SetByCaller.DexASR", "Weapon : blade dexterity scaling (dexASR).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_DexASRPlus, "Weapon.SetByCaller.DexASRPlus", "Weapon : blade dexterity scaling plus (dexASRplus).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_MgcASR, "Weapon.SetByCaller.MgcASR", "Weapon : blade magic scaling (mgcASR).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_MgcASRPlus, "Weapon.SetByCaller.MgcASRPlus", "Weapon : blade magic scaling plus (mgcASRplus).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_UseSTA, "Weapon.SetByCaller.UseSTA", "Weapon : blade stamina usage (useSTA).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_AttackSpeed, "Weapon.SetByCaller.AttackSpeed", "Weapon : blade attack speed modifier (asTime).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_StanceATK, "Weapon.SetByCaller.StanceATK", "Weapon : blade stance attack (stanceATK).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_GripAttackSpeed, "Weapon.SetByCaller.GripAttackSpeed", "Weapon : grip attack speed modifier (attackspeed).");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_SetByCaller_GripStanceATKTime, "Weapon.SetByCaller.GripStanceATKTime", "Weapon : grip stance attack time (stanceATKtime).");

	// Ability SetByCaller
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_SetByCaller_CooldownTime, "Ability.SetByCaller.CooldownTime", "Ability : ability cooldown time set by caller.");
	
	
	/** Monster Tags */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Monster_Weapon, "Monster.Weapon", "Monster : monster's weapon.");
	
	/** Shared Tags */

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeHit_Start, "Shared.Event.MeleeHit.Start", "Shared : melee hit event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_MeleeHit_End , "Shared.Event.MeleeHit.End", "Shared : melee hit end event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_Parry_Perfect, "Shared.Event.Parry.Perfect", "Shared : perfect parry event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_Parry_Success, "Shared.Event.Parry.Success", "Shared : parry success event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_Parry_Fail, "Shared.Event.Parry.Fail", "Shared : parry fail event.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_Event_AttackParried, "Shared.Event.AttackParried", "Shared : attack was parried event.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage", "Shared : base damage set by caller.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_State_LockedOn, "Shared.State.LockedOn", "Shared : player is locked onto a target.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_State_Parrying, "Shared.State.Parrying", "Shared : player is in a parrying state.");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_State_CanNotBeParried, "Shared.State.CanNotBeParried", "Shared : attack can not be parried.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_DamageType_Physical, "Shared.DamageType.Physical", "Shared : physical damage type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_DamageType_Fire, "Shared.DamageType.Fire", "Shared : fire damage type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_DamageType_Magic, "Shared.DamageType.Magic", "Shared : magic damage type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_DamageType_Holy, "Shared.DamageType.Holy", "Shared : holy damage type.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_ResistType_Physical, "Shared.ResistType.Physical", "Shared : physical resist type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_ResistType_Fire, "Shared.ResistType.Fire", "Shared : fire resist type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_ResistType_Magic, "Shared.ResistType.Magic", "Shared : magic resist type.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shared_ResistType_Holy, "Shared.ResistType.Holy", "Shared : holy resist type.");

	/** Item Tags */

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Dev, "Item.Dev", "Item : developer item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Consumable, "Item.Consumable", "Item : consumable item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Quest, "Item.Quest", "Item : quest item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Junk, "Item.Junk", "Item : junk item.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Armor, "Item.Armor", "Item : armor item.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon, "Item.Weapon", "Item : weapon item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Simple, "Item.Weapon.Simple", "Item : simple weapon item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Crafted, "Item.Weapon.Crafted", "Item : crafted weapon item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_WeaponPart, "Item.WeaponPart", "Item : weapon part.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_WeaponPart_Blade, "Item.WeaponPart.Blade", "Item : weapon part blade.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_WeaponPart_Grip, "Item.WeaponPart.Handle", "Item : weapon part handle.");
	// UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_WeaponPart_Pommel, "Item.WeaponPart.Pommel", "Item : weapon part pommel.");



}

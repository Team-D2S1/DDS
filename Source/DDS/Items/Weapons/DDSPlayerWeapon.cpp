// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Items/Weapons/DDSPlayerWeapon.h"

#include "Net/UnrealNetwork.h"

ADDSPlayerWeapon::ADDSPlayerWeapon()
{
	
}

void ADDSPlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDSPlayerWeapon, PlayerWeaponData);
	DOREPLIFETIME(ADDSPlayerWeapon, GrantedAbilitySpecHandles);
}

void ADDSPlayerWeapon::AssignGrantedAbilitySpecHandles(
	const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ADDSPlayerWeapon::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}

void ADDSPlayerWeapon::OnRep_PlayerWeaponData()
{
	
}


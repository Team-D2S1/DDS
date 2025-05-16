// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSSimplePlayerWeapon.h"

#include "Net/UnrealNetwork.h"

ADDSSimplePlayerWeapon::ADDSSimplePlayerWeapon()
{
	
}

void ADDSSimplePlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDSSimplePlayerWeapon, PlayerWeaponData);
	DOREPLIFETIME(ADDSSimplePlayerWeapon, GrantedAbilitySpecHandles);
}

void ADDSSimplePlayerWeapon::AssignGrantedAbilitySpecHandles(
	const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = InGrantedAbilitySpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ADDSSimplePlayerWeapon::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}

void ADDSSimplePlayerWeapon::OnRep_PlayerWeaponData()
{
	
}


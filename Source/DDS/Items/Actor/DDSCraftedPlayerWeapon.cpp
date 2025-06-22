// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Items/Actor/DDSCraftedPlayerWeapon.h"

#include "Components/BoxComponent.h"
#include "ETC/CustomLog.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Net/UnrealNetwork.h"

ADDSCraftedPlayerWeapon::ADDSCraftedPlayerWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADDSCraftedPlayerWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDSCraftedPlayerWeapon, WeaponItemInstance);
}




void ADDSCraftedPlayerWeapon::SetBladeItemInstance(UItemInstance* NewBladeItemInstance)
{
	WeaponItemInstance->SetBladeItemInstance(NewBladeItemInstance);
}

void ADDSCraftedPlayerWeapon::SetGripItemInstance(UItemInstance* NewGripItemInstance)
{
	WeaponItemInstance->SetGripItemInstance(NewGripItemInstance);
}

void ADDSCraftedPlayerWeapon::SetPommelItemInstance(UItemInstance* NewPommelItemInstance)
{
	WeaponItemInstance->SetPommelItemInstance(NewPommelItemInstance);
}

UItemInstance* ADDSCraftedPlayerWeapon::GetBladeItemInstance() const
{
	if (WeaponItemInstance)
	{
		return WeaponItemInstance->GetBladeItemInstance();
	}
	MY_ERROR_DISPLAY_NET(HasAuthority(),TEXT("WeaponItemInstance is nullptr"));
	return nullptr;
}

UItemInstance* ADDSCraftedPlayerWeapon::GetGripItemInstance() const
{
	if (WeaponItemInstance)
	{
		return WeaponItemInstance->GetGripItemInstance();
	}
	MY_ERROR_DISPLAY_NET(HasAuthority(), TEXT("WeaponItemInstance is nullptr"));
	return nullptr;
}

UItemInstance* ADDSCraftedPlayerWeapon::GetPommelItemInstance() const
{
	if (WeaponItemInstance)
	{
		return WeaponItemInstance->GetPommelItemInstance();
	}
	MY_ERROR_DISPLAY_NET(HasAuthority(), TEXT("WeaponItemInstance is nullptr"));
	return nullptr;
}


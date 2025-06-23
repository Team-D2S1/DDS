// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Items/Actor/DDSCraftedPlayerWeapon.h"

#include "WeaponBladePart.h"
#include "Components/BoxComponent.h"
#include "ETC/CustomLog.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Net/UnrealNetwork.h"

ADDSCraftedPlayerWeapon::ADDSCraftedPlayerWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetReplicates(true);
}

void ADDSCraftedPlayerWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADDSCraftedPlayerWeapon, WeaponItemInstance);
	DOREPLIFETIME(ADDSCraftedPlayerWeapon, BladePartActor);
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

void ADDSCraftedPlayerWeapon::OnBladeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADDSCraftedPlayerWeapon::OnBladeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

UBoxComponent* ADDSCraftedPlayerWeapon::GetWeaponCollsionBox() const
{
	if (BladePartActor)
	{
		return BladePartActor->GetBladePartCollisionBox();
	}
	bool bIsServer = HasAuthority();
	MY_LOG(LogTemp, Error, TEXT("[%s] BladePartActor is nullptr"), bIsServer ? TEXT("Server") : TEXT("Client"));
	return WeaponCollisionBox;
}

void ADDSCraftedPlayerWeapon::SetBladeActor(AWeaponBladePart* NewBladePartActor)
{
	if (NewBladePartActor)
	{
		MY_LOG(LogTemp, Log, TEXT("Setting BladePartActor %s To %s"), *NewBladePartActor->GetName(), *GetName());
		BladePartActor = NewBladePartActor;
		WeaponCollisionBox = BladePartActor->GetBladePartCollisionBox();
		BladePartActor->OnWeaponBladePartBeginOverlap.AddDynamic(this, &ADDSCraftedPlayerWeapon::OnBladeBeginOverlap);
		BladePartActor->OnWeaponBladePartEndOverlap.AddDynamic(this, &ADDSCraftedPlayerWeapon::OnBladeEndOverlap);
	}
	else
	{
		MY_ERROR_DISPLAY(TEXT("NewBladePartActor is nullptr"));
	}
}

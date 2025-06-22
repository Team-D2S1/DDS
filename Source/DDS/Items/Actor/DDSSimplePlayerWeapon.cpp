// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSSimplePlayerWeapon.h"

#include "Character/Player/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "DDSTypes/DDSClassTypes.h"
#include "Items/ItemInstance/ItemInstance.h"
#include "Net/UnrealNetwork.h"

ADDSSimplePlayerWeapon::ADDSSimplePlayerWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox-> SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADDSSimplePlayerWeapon::OnBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ADDSSimplePlayerWeapon::OnEndOverlap);

	if (DefaultWeaponItemClass && DefaultWeaponItemClass.Get() && WeaponItemInstance == nullptr)
	{
		if (HasAuthority())
		{
			WeaponItemInstance = NewObject<UItemInstance>();
			if (WeaponItemInstance->Init(DefaultWeaponItemClass))
			{
				return;
			}
		}
	}
}

void ADDSSimplePlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADDSSimplePlayerWeapon, GrantedAbilitySpecHandles);
}



void ADDSSimplePlayerWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADDSSimplePlayerWeapon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


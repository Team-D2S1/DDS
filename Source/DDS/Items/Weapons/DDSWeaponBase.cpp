// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Items/Weapons/DDSWeaponBase.h"

#include "Character/Player/PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADDSWeaponBase::ADDSWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox-> SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADDSWeaponBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ADDSWeaponBase, OwnerPawn);
	DOREPLIFETIME(ADDSWeaponBase, WeaponTag);
}

void ADDSWeaponBase::SetOwnerPawn(APawn* InOwnerPawn)
{
	OwnerPawn = InOwnerPawn;
}

void ADDSWeaponBase::OnRep_OwnerPawn()
{
	if (OwnerPawn)
	{
		APlayerBase* Player = Cast<APlayerBase>(OwnerPawn);
		if (Player)
		{
			Player->GetCombatComponent()->RegisterSpawnedWeapon(WeaponTag ,this);
		}
	}
}

void ADDSWeaponBase::SetWeaponTag(FGameplayTag InWeaponTag)
{
	WeaponTag = InWeaponTag;
}

void ADDSWeaponBase::OnRep_WeaponTag()
{
}

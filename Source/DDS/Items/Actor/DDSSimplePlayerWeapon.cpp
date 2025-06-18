// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSSimplePlayerWeapon.h"

#include "Components/BoxComponent.h"
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

void ADDSSimplePlayerWeapon::OnRep_PlayerWeaponData()
{
	
}


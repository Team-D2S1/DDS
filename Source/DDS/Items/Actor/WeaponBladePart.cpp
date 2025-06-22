// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "Items/Actor/WeaponBladePart.h"

#include "Components/BoxComponent.h"


// Sets default values
AWeaponBladePart::AWeaponBladePart()
{
	PrimaryActorTick.bCanEverTick = false;

	BladePartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladePartMesh"));
	SetRootComponent(BladePartMesh);
	BladePartMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BladePartCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BladePartCollisionBox"));
	BladePartCollisionBox->SetupAttachment(RootComponent);
	BladePartCollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BladePartCollisionBox->SetGenerateOverlapEvents(true);
	BladePartCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBladePart::OnBeginOverlap);
	BladePartCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AWeaponBladePart::OnEndOverlap);
}

// Called when the game starts or when spawned
void AWeaponBladePart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBladePart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnWeaponBladePartBeginOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, SweepResult);
}

void AWeaponBladePart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnWeaponBladePartEndOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


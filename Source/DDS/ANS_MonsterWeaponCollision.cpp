// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "ANS_MonsterWeaponCollision.h"

#include "Character/Monster/MonsterBase.h"
#include "Character/Player/PlayerBase.h"
#include "Components/Combat/MonsterCombatComponent.h"


void UANS_MonsterWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	HitActors.Empty();
}

void UANS_MonsterWeaponCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	PerformWeaponTrace(MeshComp);
}

void UANS_MonsterWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	HitActors.Empty();
}

void UANS_MonsterWeaponCollision::PerformWeaponTrace(USkeletalMeshComponent* MeshComp)
{
	if(!MeshComp) return;

	if(!MeshComp->GetOwner()->HasAuthority()) return; 

	AMonsterBase* Monster = Cast<AMonsterBase>(MeshComp->GetOwner());
	if(!Monster) return;
	
	const USkeletalMeshComponent* WeaponMesh = Monster->GetWeaponMesh();

	FVector StartLocation = WeaponMesh->GetSocketLocation(WeaponStartSocketName);
	FVector EndLocation = WeaponMesh->GetSocketLocation(WeaponEndSocketName);

	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(5.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Monster);
	Params.bTraceComplex = false;

	bool bHit = Monster->GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		Sphere,
		Params);

	if(true)
	{
		DrawDebugCapsule(
			Monster->GetWorld(),
			(StartLocation + EndLocation) / 2.f,
			FVector::Dist(StartLocation, EndLocation) / 2.f,
			5.f,
			FRotationMatrix::MakeFromZ(EndLocation - StartLocation).ToQuat(),
			bHit ? FColor::Red : FColor::Green,
			false,
			0.1f);
	}

	if(bHit)
	{
		for(const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();

			if(!HitActor || HitActors.Contains(HitActor)) continue;

			HitActors.Add(HitActor);

			if(Cast<APlayerBase>(HitActor)) // 플레이어라면
			{
				Monster->GetMonsterCombatComponent()->ApplyDamageToTarget(Cast<APlayerBase>(HitActor));
			}
		}
	}
}

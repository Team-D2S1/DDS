// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "ANS_SwordTrail.h"

#include "Character/Monster/MonsterBase.h"


void UANS_SwordTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(!SwordTrailEffect) return;

	if(AMonsterBase* Monster = Cast<AMonsterBase>(MeshComp->GetOwner()))
	{
		USkeletalMeshComponent* WeaponMesh = const_cast<USkeletalMeshComponent*>(Monster->GetWeaponMesh()); 
		if(WeaponMesh)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			SwordTrailEffect,
			WeaponMesh,
			NAME_None, // 특정 소켓에 붙이지 않음
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
			);

			if(NiagaraComp)
			{
				
			}
		}
	}
}

void UANS_SwordTrail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UANS_SwordTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}

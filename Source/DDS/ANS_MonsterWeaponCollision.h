// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MonsterWeaponCollision.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UANS_MonsterWeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY()
	FName WeaponStartSocketName = "WeaponStart";

	UPROPERTY()
	FName WeaponEndSocketName = "WeaponEnd";

private:
	TSet<AActor*> HitActors;

	void PerformWeaponTrace(USkeletalMeshComponent* MeshComp);
};

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ANS_SwordTrail.generated.h"

/**
 * 
 */
UCLASS()
class DDS_API UANS_SwordTrail : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponStartSocketName = "WeaponStart";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponEndSocketName = "WeaponEnd";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* SwordTrailEffect;

private:
};

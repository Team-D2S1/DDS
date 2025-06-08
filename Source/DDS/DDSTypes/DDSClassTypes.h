// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DDSStructTypes.h"
#include "GameplayTagContainer.h"
#include "PaperSprite.h"
#include "DDSClassTypes.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UItemStaticData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer ItemStaticTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperSprite> ItemIcon;


	// 무기 파트 데이터
	// ItemTypeTag에 따른 파트는 하나지만, 세개다 선언은 해둠.

	// 무기 메시
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> WeaponMesh;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBladePartData BladeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGripPartData GripData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPommelPartData PommelData;
	
};

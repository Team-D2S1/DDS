// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GripData.generated.h"

/**
 * Grip(자루) 데이터 테이블: 공격 모션, 공격 속도, 강인도 보정
 */
USTRUCT(BlueprintType)
struct DDS_API FGripData : public FTableRowBase
{
	GENERATED_BODY()

public:

	// 고유 식별자 (예: "straightSword", "greatSword")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
	FName ID;
	
	// 한국어 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
	FName StatID;

	// 공격 모션(AnimSequence / Montage 등 파일명 키)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
	FName GripMoveSet;

	// 공격 속도 배율(attackspeed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
	float AttackSpeed = 1.f;

	// 강인도 보정 시간 배율(stanceATKtime)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grip")
	float StanceATKTime = 1.f;
};


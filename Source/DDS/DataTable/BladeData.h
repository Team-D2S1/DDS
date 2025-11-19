// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BladeData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct DDS_API FBladeData : public FTableRowBase 
{
	GENERATED_BODY()

public:


	// 고유 식별자 (예: "straightSword", "greatSword")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	FName ID;
	
	// 한국어 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	FName StatID;

	// 표기 공격력(BaseATK)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	int32 BaseATK = 0;

	// 표기 공격력 증가량(BaseATKplus)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	int32 BaseATKplus = 0;

	// 근력 보정(powASR)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float PowASR = 0.f;

	// 근력 보정 증가량(powASRplus)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float PowASRplus = 0.f;

	// 기량 보정(aglASR)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float AglASR = 0.f;

	// 기량 보정 증가량(aglASRplus)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float AglASRplus = 0.f;

	// 마법 보정(mgcASR)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float MgcASR = 0.f;

	// 마법 보정 증가량(mgcASRplus)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float MgcASRplus = 0.f;

	// 스태미나 소모량(useSTA)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	int32 UseSTA = 0;

	// 공격 속도 비율(asTime)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	float AsTime = 1.f;

	// 강인도 감쇄(stanceATK)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blade")
	int32 StanceATK = 0;
};

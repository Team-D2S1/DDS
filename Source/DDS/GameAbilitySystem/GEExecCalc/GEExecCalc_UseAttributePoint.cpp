// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#include "GameAbilitySystem/GEExecCalc/GEExecCalc_UseAttributePoint.h"
#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAttributeSet.h"

struct FAttributePointCapture
{
	// Attribute 캡처 정의
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttributePoints);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vitality);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Magic);
	
	FAttributePointCapture()
	{
		// Target(자기자신)의 Attribute들을 캡처
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, AttributePoints, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Vitality, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Endurance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Strength, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Dexterity, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDDSAttributeSet, Magic, Target, false);
	}
};

static const FAttributePointCapture& GetAttributePointCapture()
{
	static FAttributePointCapture AttributePointCapture;
	return AttributePointCapture;
}

UGEExecCalc_UseAttributePoint::UGEExecCalc_UseAttributePoint()
{
	// 캡처할 Attribute 등록
	RelevantAttributesToCapture.Add(GetAttributePointCapture().AttributePointsDef);
	RelevantAttributesToCapture.Add(GetAttributePointCapture().VitalityDef);
	RelevantAttributesToCapture.Add(GetAttributePointCapture().EnduranceDef);
	RelevantAttributesToCapture.Add(GetAttributePointCapture().StrengthDef);
	RelevantAttributesToCapture.Add(GetAttributePointCapture().DexterityDef);
	RelevantAttributesToCapture.Add(GetAttributePointCapture().MagicDef);
}

void UGEExecCalc_UseAttributePoint::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	using namespace DDSGameplayTags;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	if (!TargetASC || !SourceASC)
	{
		MY_LOG_DISPLAY("ASC is null");
		return;
	}

	AActor* TargetActor = TargetASC->GetAvatarActor();
	if (!TargetActor)
	{
		MY_LOG_DISPLAY("TargetActor is null");
		return;
	}

	// FAggregatorEvaluateParameters 설정
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 현재 AttributePoints 값 가져오기
	float CurrentAttributePoints = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetAttributePointCapture().AttributePointsDef, 
		EvaluationParameters, 
		CurrentAttributePoints
	);

	// AttributePoint가 충분한지 확인
	if (CurrentAttributePoints < 1.f)
	{
		bool bIsServer = TargetActor->HasAuthority();
		MY_CLOG_DISPLAY_NET(FColor::Red, bIsServer, TEXT("Not enough AttributePoints! Current: %.0f"), CurrentAttributePoints);
		return;
	}

	// SetByCaller로 전달된 태그들을 확인하여 어느 Attribute를 증가시킬지 결정
	FGameplayTag AttributeToUpgrade;
	FGameplayAttribute AttributeToModify;

	// 모든 Primary Attribute 태그를 확인
	TArray<FGameplayTag> PrimaryAttributeTags = {
		Attribute_Primary_Vitality,
		Attribute_Primary_Endurance,
		Attribute_Primary_Strength,
		Attribute_Primary_Dexterity,
		Attribute_Primary_Magic
	};

	for (const FGameplayTag& Tag : PrimaryAttributeTags)
	{
		// SetByCaller로 해당 태그에 값이 설정되었는지 확인
		const float* Magnitude = Spec.SetByCallerTagMagnitudes.Find(Tag);
		if (Magnitude != nullptr && *Magnitude > 0.f)
		{
			AttributeToUpgrade = Tag;
			break;
		}
	}

	// 업그레이드할 Attribute가 없으면 종료
	if (!AttributeToUpgrade.IsValid())
	{
		bool bIsServer = TargetActor->HasAuthority();
		MY_CLOG_DISPLAY_NET(FColor::Orange, bIsServer, TEXT("No valid attribute tag found in SetByCaller"));
		return;
	}

	// 태그에 따라 Attribute 결정
	if (AttributeToUpgrade.MatchesTagExact(Attribute_Primary_Vitality))
	{
		AttributeToModify = UDDSAttributeSet::GetVitalityAttribute();
	}
	else if (AttributeToUpgrade.MatchesTagExact(Attribute_Primary_Endurance))
	{
		AttributeToModify = UDDSAttributeSet::GetEnduranceAttribute();
	}
	else if (AttributeToUpgrade.MatchesTagExact(Attribute_Primary_Strength))
	{
		AttributeToModify = UDDSAttributeSet::GetStrengthAttribute();
	}
	else if (AttributeToUpgrade.MatchesTagExact(Attribute_Primary_Dexterity))
	{
		AttributeToModify = UDDSAttributeSet::GetDexterityAttribute();
	}
	else if (AttributeToUpgrade.MatchesTagExact(Attribute_Primary_Magic))
	{
		AttributeToModify = UDDSAttributeSet::GetMagicAttribute();
	}
	else
	{
		bool bIsServer = TargetActor->HasAuthority();
		MY_CLOG_DISPLAY_NET(FColor::Red, bIsServer, TEXT("Unknown attribute tag: %s"), *AttributeToUpgrade.ToString());
		return;
	}

	// AttributePoints 감소 (1 포인트 소모)
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UDDSAttributeSet::GetAttributePointsAttribute(),
		EGameplayModOp::Additive,
		-1.f
	));

	// 선택된 Attribute 증가 (1 증가)
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		AttributeToModify,
		EGameplayModOp::Additive,
		1.f
	));

	// bool bIsServer = TargetActor->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Green, bIsServer, 
	// 	TEXT("Used AttributePoint! Upgraded: %s | Remaining Points: %.0f"), 
	// 	*AttributeToUpgrade.ToString(), 
	// 	CurrentAttributePoints - 1.f
	// );
}


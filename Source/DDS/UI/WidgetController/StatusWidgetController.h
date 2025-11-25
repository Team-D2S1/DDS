// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/DDSWidgetController.h"
#include "AttributeSet.h"
#include "StatusWidgetController.generated.h"

struct FDDSAttributeInfo;
class UAttributeInfo;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FDDSAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributePointsChangedSignature, int32, AttributePoints);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DDS_API UStatusWidgetController : public UDDSWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="GAS|Attributes")
	virtual void BroadcastInitialValue() override;

	UFUNCTION(BlueprintCallable, Category="GAS|Attributes")
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributePointsChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable, Category="GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Attributes")
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};

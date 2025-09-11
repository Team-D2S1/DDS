// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FDDSAttributeInfo
{
	GENERATED_BODY()
public:
	FDDSAttributeInfo() = default;
	FDDSAttributeInfo(FGameplayTag InAttributeTag, const FString& InAttributeName)
		: AttributeTag(InAttributeTag)
		, AttributeName(FText::FromString(InAttributeName))
	{}

	FDDSAttributeInfo(FGameplayTag InAttributeTag, const FString& InAttributeName, const FString& InAttributeDescription, float InAttributeValue = 0.0f)
		: AttributeTag(InAttributeTag)
		, AttributeName(FText::FromString(InAttributeName))
		, AttributeDescription(FText::FromString(InAttributeDescription))
		, AttributeValue(InAttributeValue)
	{}

	FDDSAttributeInfo(FGameplayTag InAttributeTag, const FText& InAttributeName, const FText& InAttributeDescription, float InAttributeValue = 0.0f)
		: AttributeTag(InAttributeTag)
		, AttributeName(InAttributeName)
		, AttributeDescription(InAttributeDescription)
		, AttributeValue(InAttributeValue)
	{}

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText::FromString("AttributeName");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText::FromString("AttributeDescription");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttributeValue = 0.0f;
	
};



/**
 * 
 */
UCLASS()
class DDS_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UAttributeInfo();
	

	FDDSAttributeInfo FindAttributeInfoByTag(const FGameplayTag& InTag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DDS|AttributeInfo")
	TArray<FDDSAttributeInfo> AttributeInformations;

	
	
};

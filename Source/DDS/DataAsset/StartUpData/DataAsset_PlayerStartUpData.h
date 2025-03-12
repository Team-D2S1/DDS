// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DDSTypes/DDSStructTypes.h"
#include "DataAsset_PlayerStartUpData.generated.h"

class UDDSPlayerGameplayAbility;
// class UDDSGameplayAbility;


/**
 * 
 */
UCLASS()
class DDS_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData",meta = (TitleProperty = "InputTag"))
	TArray<FDDSPlayerAbilitySet> PlayerStartUpAbilitiesSet;
};

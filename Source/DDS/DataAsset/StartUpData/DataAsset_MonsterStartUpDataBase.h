// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_MonsterStartUpDataBase.generated.h"

class UDDSMonsterGameplayAbility;
/**
 * 
 */
UCLASS()
class DDS_API UDataAsset_MonsterStartUpDataBase : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UDDSMonsterGameplayAbility>> MonsterCombatAbilities;
	
	
};

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameAbilitySystem/Abilities/DDSPlayerGameplayAbility.h"
#include "DDSMonsterGameplayAbility.generated.h"


class UMonsterCombatComponent;
class AMonsterBase;
/**
 * 
 */
UCLASS()
class DDS_API UDDSMonsterGameplayAbility : public UDDSPlayerGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	AMonsterBase* GetMonsterCharacterFromActorInfo();

	// UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	// AInGamePlayerController* GetMonsterControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	UMonsterCombatComponent* GetMonsterCombatComponentFromActorInfo();
private:
	
	TWeakObjectPtr<AMonsterBase> CachedMonsterCharacter;
	// TWeakObjectPtr<AInGamePlayerController> CachedMonsterController;
		
	
};

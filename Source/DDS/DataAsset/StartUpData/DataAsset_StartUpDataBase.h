// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UDDSAbilitySystemComponent;
class UDDSGameplayAbility;
/**
 * 
 */
UCLASS()
class DDS_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// ASC에 능력을 부여하는 함수
	virtual void GiveToAbilitySystemComponent(UDDSAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);
protected:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UDDSGameplayAbility>> ActivateOnGivenAbilities;

	// 특정 조건시 반응하는 능력(피격, 사망 등)
	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UDDSGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>> InAbilitiesToGive,UDDSAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);
};

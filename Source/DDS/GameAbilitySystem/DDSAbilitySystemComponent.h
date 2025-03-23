// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DDSAbilitySystemComponent.generated.h"

struct FDDSPlayerAbilitySet;
class UDDSGameplayAbility;
/**
 * 
 */
UCLASS()
class DDS_API UDDSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>>& StartupAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable,Category="DDS|Ability",meta=(ApplyLevel=1))
	void GrantPlayerWeaponAbilities(const TArray<FDDSPlayerAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutHandles);
	UFUNCTION(BlueprintCallable,Category="DDS|Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InHandles);


	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AddLooseGameplayTag(const FGameplayTag& InTag);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_RemoveLooseGameplayTag(const FGameplayTag& InTag);
	// TODO 이펙트 적용시 클라이언트에게 알림
	// UFUNCTION(Client, Reliable)
	// void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};



// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDSGameplayAbility.generated.h"

class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EDDSAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven // 부여 즉시 발동, 발동 종료시 능력 삭제
};

/**
 * 
 */
UCLASS()
class DDS_API UDDSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="DDSAbility")
	EDDSAbilityActivationPolicy AbilityActivationPolicy = EDDSAbilityActivationPolicy::OnTriggered;

	// ~ Begin UGameplayAbility
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// ~ End UGameplayAbility

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UPawnCombatComponent* GetPawnCombatComponent() const;
public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FORCEINLINE EDDSAbilityActivationPolicy GetAbilityActivationPolicy() const { return AbilityActivationPolicy; }
};

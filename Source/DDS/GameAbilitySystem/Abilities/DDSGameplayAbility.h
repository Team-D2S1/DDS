// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DDSTypes/DDSEnumTypes.h"
#include "DDSGameplayAbility.generated.h"

class UPawnCombatComponent;
class UDDSAbilitySystemComponent;

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

	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	UDDSAbilitySystemComponent* GetDDSAbilitySystemComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "DDS|Ability")
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InEffectSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "DDS|Ability",meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor",ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InEffectSpecHandle,EDDSSuccessType& OutSuccessType);


	/**
	 * 콤보 카운드가 SetByCallerMagnitude 로 설정됨
	 * @param EffectClass 
	 * @param InWeaponBaseDamage 
	 * @param InCurrentAttackTypeTag 
	 * @param InCurrentAttackComboCount 
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	FGameplayEffectSpecHandle MakeGameplayEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag, int32 InCurrentAttackComboCount) const;
public:
	UFUNCTION(BlueprintPure, Category = "DDS|Ability")
	FORCEINLINE EDDSAbilityActivationPolicy GetAbilityActivationPolicy() const { return AbilityActivationPolicy; }
	

protected:
	// void PlayMontageAndWaitForEvent(UAnimMontage* Montage, FName SectionName, FGameplayTag EventTag, float Rate = 1.f, FName StartSection = NAME_None);

};

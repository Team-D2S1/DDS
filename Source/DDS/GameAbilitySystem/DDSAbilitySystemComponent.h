// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DDSAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxStaminaChangedSignature, float, NewMaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSMaxManaChangedSignature, float, NewMaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDDSDamageTakenChangedSignature, float, NewDamageTaken);

struct FDDSPlayerAbilitySet;
class UDDSGameplayAbility;
class UDDSAttributeSet;
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
	
	/** AttributeSet 값 변경 델리게이트 바인딩 */
	void BindAttributeValueChangeDelegates(UDDSAttributeSet* InAttributeSet);

	// 게임 전역에서 사용 가능한 Attribute 변경 델리게이트 (UI, 이펙트, 사운드 등 모두 구독 가능)
	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSStaminaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxStaminaChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="DDS|Attributes")
	FOnDDSDamageTakenChangedSignature OnDamageTakenChanged;

protected:
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleManaChanged(const FOnAttributeChangeData& Data);
	void HandleHealthMaxChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaMaxChanged(const FOnAttributeChangeData& Data);
	void HandleManaMaxChanged(const FOnAttributeChangeData& Data);
	void HandleDamageTakenChanged(const FOnAttributeChangeData& Data);
};

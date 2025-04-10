// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "DDSAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;
/**
 * 기본 어트리부트 세트.
 * 강의대로 만드는 중
 *
 * TODO : Health는 전용 AttributeSet 로 분리해도 될듯
 * https://dev.epicgames.com/community/learning/tutorials/DPpd/unreal-engine-gameplay-ability-system-best-practices-for-setup#howdoilettheplayeractivateabilities?
 */ 
UCLASS()
class DDS_API UDDSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDDSAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Health);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MaxHealth);
	
	UPROPERTY(ReplicatedUsing = OnRep_Stamina, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, Stamina);

	UPROPERTY(ReplicatedUsing= OnRep_MaxStamina, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, MaxStamina);

    UPROPERTY(ReplicatedUsing= OnRep_AttackPower, BlueprintReadOnly, Category = "Combat Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, AttackPower);

    UPROPERTY(ReplicatedUsing= OnRep_DefensePower, BlueprintReadOnly, Category = "Combat Attributes")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DefensePower);

	UPROPERTY(ReplicatedUsing= OnRep_DamageTaken, BlueprintReadOnly, Category = "Combat Attributes")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UDDSAttributeSet, DamageTaken);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;

	UFUNCTION()
	void OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower) const;

	UFUNCTION()
	void OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const;


private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};






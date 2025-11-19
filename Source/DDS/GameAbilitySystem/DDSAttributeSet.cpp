// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "DDSGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include  "GameplayEffectExtension.h"
#include "NativeGameplayTags.h"
#include "Components/UI/PawnUIComponent.h"
#include "ETC/CustomLog.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PlayerUIComponent.h"



UDDSAttributeSet::UDDSAttributeSet()
{
    // 기본값 설정
    InitLevel(1.0f);
    InitEnergy(0.0f);
    InitRequireEnergy(0.0f);
    InitSoul(0.0f);
    
    InitVitality(10.0f);
    InitEndurance(10.0f);
	InitStrength(10.0f);
    InitDexterity(10.0f);
    InitMagic(10.0f);
    
    InitHealth(100.0f);
    InitHealthMax(100.0f);
    InitStamina(100.0f);
    InitStaminaMax(100.0f);
    InitMana(100.0f);
    InitManaMax(100.0f);
    
    InitStrengthAR(1.0f);
    InitStrengthARPlus(0.0f);
    InitDexterityAR(1.0f);
    InitDexterityARPlus(0.0f);
    InitMagicAR(1.0f);
    InitMagicARPlus(0.0f);
    
    InitPhysicalASR(0.0f);
    InitDexterityASR(0.0f);
    InitMagicASR(0.0f);
    
    InitAttackPower(10.0f);
    InitMagicPower(10.0f);
    
    InitPhysicalDefense(5.0f);
    InitMagicDefense(5.0f);
    InitPhysicalResist(0.0f);
	InitMagicResist(0.0f);
    
    InitDamageTaken(0.0f);
    InitAttackRequireStamina(0.0f);
 
	using namespace DDSGameplayTags;
    
    TagToAttributeMap.Add(Attribute_Default_Level, GetLevelAttribute);
    TagToAttributeMap.Add(Attribute_Default_Energy, GetEnergyAttribute);
    TagToAttributeMap.Add(Attribute_Default_RequireEnergy, GetRequireEnergyAttribute);
    TagToAttributeMap.Add(Attribute_Default_Soul, GetSoulAttribute);
    
    TagToAttributeMap.Add(Attribute_Primary_Vitality, GetVitalityAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Endurance, GetEnduranceAttribute);
	TagToAttributeMap.Add(Attribute_Primary_Strength, GetStrengthAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Dexterity, GetDexterityAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Magic, GetMagicAttribute);
    
    TagToAttributeMap.Add(Attribute_Status_Health, GetHealthAttribute);
    TagToAttributeMap.Add(Attribute_Status_Stamina, GetStaminaAttribute);
    TagToAttributeMap.Add(Attribute_Status_Mana, GetManaAttribute);
    TagToAttributeMap.Add(Attribute_Status_HealthMax, GetHealthMaxAttribute);
    TagToAttributeMap.Add(Attribute_Status_StaminaMax, GetStaminaMaxAttribute);
    TagToAttributeMap.Add(Attribute_Status_ManaMax, GetManaMaxAttribute);
    
    TagToAttributeMap.Add(Attribute_Offense_StrengthAR, GetStrengthARAttribute);
    TagToAttributeMap.Add(Attribute_Offense_StrengthARPlus, GetStrengthARPlusAttribute);
    TagToAttributeMap.Add(Attribute_Offense_DexterityAR, GetDexterityARAttribute);
    TagToAttributeMap.Add(Attribute_Offense_DexterityARPlus, GetDexterityARPlusAttribute);
    TagToAttributeMap.Add(Attribute_Offense_MagicAR, GetMagicARAttribute);
    TagToAttributeMap.Add(Attribute_Offense_MagicARPlus, GetMagicARPlusAttribute);
    TagToAttributeMap.Add(Attribute_Offense_PhysicalASR, GetPhysicalASRAttribute);
    TagToAttributeMap.Add(Attribute_Offense_DexterityASR, GetDexterityASRAttribute);
    TagToAttributeMap.Add(Attribute_Offense_MagicASR, GetMagicASRAttribute);
    TagToAttributeMap.Add(Attribute_Offense_PhysicalPower, GetAttackPowerAttribute);
    TagToAttributeMap.Add(Attribute_Offense_MagicPower, GetMagicPowerAttribute);
    
    TagToAttributeMap.Add(Attribute_Defense_PhysicalDefense, GetPhysicalDefenseAttribute);
    TagToAttributeMap.Add(Attribute_Defense_MagicDefense, GetMagicDefenseAttribute);
    TagToAttributeMap.Add(Attribute_Defense_PhysicalResist, GetPhysicalResistAttribute);
	TagToAttributeMap.Add(Attribute_Defense_MagicResist, GetMagicResistAttribute);
}

void UDDSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Level, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Energy, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, RequireEnergy, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Soul, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Magic, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, StaminaMax, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, StrengthAR, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, StrengthARPlus, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, DexterityAR, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, DexterityARPlus, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicAR, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicARPlus, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, PhysicalASR, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, DexterityASR, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicASR, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicDefense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, PhysicalResist, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicResist, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, AttackRequireStamina, COND_None, REPNOTIFY_Always);
}

void UDDSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 1차 능력치 최소/최대는 1,99
	// 어차피 base만 바뀌는 거라 PostGameplayEffectExecute에서 실행해도 무방
	if (Attribute == GetVitalityAttribute() ||
		Attribute == GetEnduranceAttribute() ||
		Attribute == GetStrengthAttribute() ||
		Attribute == GetDexterityAttribute() ||
		Attribute == GetMagicAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 99.f);
	}
}

void UDDSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	bool bHasAuthority = false;
	if (const UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
	{
		if (const AActor* OwnerActor = ASC->GetOwner())
		{
			bHasAuthority = OwnerActor->HasAuthority();
		}
	}

	// Health: 클램프 및 죽음 태그 처리만 수행 (UI는 ASC 델리게이트로 처리)
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth(), 0.f, GetHealthMax());
		SetHealth(NewHealth);
		MY_CLOG_DISPLAY_NET(FColor::Green, bHasAuthority, TEXT("%s Health clamped to %f"), *GetName(), NewHealth);
	}

	// Stamina: 값 클램프만 수행
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float NewStamina = FMath::Clamp(GetStamina(), 0.f, GetStaminaMax());
		SetStamina(NewStamina);
	}

	// StaminaMax: 값 클램프만 수행
	if (Data.EvaluatedData.Attribute == GetStaminaMaxAttribute())
	{
		const float NewMaxStamina = FMath::Clamp(GetStaminaMax(), 0.f, GetStaminaMax());
		SetStaminaMax(NewMaxStamina);
	}

	// DamageTaken: 데미지 계산 + 죽음 태그 처리 (UI는 델리게이트에 맡김)
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetHealth();
		const float TakenDamage = GetDamageTaken();
		const float NewHealth = FMath::Clamp(OldHealth - TakenDamage, 0.f, GetHealthMax());
		SetHealth(NewHealth);

		MY_CLOG_DISPLAY_NET(FColor::Red, bHasAuthority, TEXT("OldHealth: %f, TakenDamage: %f, NewHealth: %f"), OldHealth, TakenDamage, NewHealth);

		if (GetHealth() <= 0.f)
		{
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead.Start"));
			}
		}
	}

	// HealthMax: 값 클램프만 수행 (UI 갱신은 ASC 델리게이트가 처리)
	if (Data.EvaluatedData.Attribute == GetHealthMaxAttribute())
	{
		const float NewMaxHealth = FMath::Clamp(GetHealthMax(), 0.f, GetHealthMax());
		SetHealthMax(NewMaxHealth);
	}
}


void UDDSAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Level, OldLevel);
}

void UDDSAttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Energy, OldEnergy);
}

void UDDSAttributeSet::OnRep_RequireEnergy(const FGameplayAttributeData& OldRequireEnergy) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, RequireEnergy, OldRequireEnergy);
}

void UDDSAttributeSet::OnRep_Soul(const FGameplayAttributeData& OldSoul) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Soul, OldSoul);
}

void UDDSAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Vitality, OldVitality);
}

void UDDSAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Strength, OldStrength);
}

void UDDSAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Endurance, OldEndurance);
}

void UDDSAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Dexterity, OldDexterity);
}

void UDDSAttributeSet::OnRep_Magic(const FGameplayAttributeData& OldMagic) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Magic, OldMagic);
}

void UDDSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Health, OldHealth);
}

void UDDSAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Stamina, OldStamina);
}

void UDDSAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Mana, OldMana);
}

void UDDSAttributeSet::OnRep_HealthMax(const FGameplayAttributeData& OldHealthMax) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, HealthMax, OldHealthMax);
}

void UDDSAttributeSet::OnRep_StaminaMax(const FGameplayAttributeData& OldStaminaMax) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, StaminaMax, OldStaminaMax);
}

void UDDSAttributeSet::OnRep_ManaMax(const FGameplayAttributeData& OldManaMax) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, ManaMax, OldManaMax);
}

void UDDSAttributeSet::OnRep_BaseAttack(const FGameplayAttributeData& OldBaseAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, BaseAttack, OldBaseAttack);
}

void UDDSAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, AttackPower, OldAttackPower);
}

void UDDSAttributeSet::OnRep_MagicPower(const FGameplayAttributeData& OldMagicPower) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicPower, OldMagicPower);
}

void UDDSAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UDDSAttributeSet::OnRep_MagicDefense(const FGameplayAttributeData& OldMagicDefense) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicDefense, OldMagicDefense);
}

void UDDSAttributeSet::OnRep_PhysicalResist(const FGameplayAttributeData& OldPhysicalResist) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, PhysicalResist, OldPhysicalResist);
}

void UDDSAttributeSet::OnRep_MagicResist(const FGameplayAttributeData& OldMagicResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicResist, OldMagicResist);
}

void UDDSAttributeSet::OnRep_StrengthAR(const FGameplayAttributeData& OldStrengthAR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, StrengthAR, OldStrengthAR);
}

void UDDSAttributeSet::OnRep_StrengthARPlus(const FGameplayAttributeData& OldStrengthARPlus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, StrengthARPlus, OldStrengthARPlus);
}

void UDDSAttributeSet::OnRep_DexterityAR(const FGameplayAttributeData& OldDexterityAR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DexterityAR, OldDexterityAR);
}

void UDDSAttributeSet::OnRep_DexterityARPlus(const FGameplayAttributeData& OldDexterityARPlus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DexterityARPlus, OldDexterityARPlus);
}

void UDDSAttributeSet::OnRep_MagicAR(const FGameplayAttributeData& OldMagicAR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicAR, OldMagicAR);
}

void UDDSAttributeSet::OnRep_MagicARPlus(const FGameplayAttributeData& OldMagicARPlus) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicARPlus, OldMagicARPlus);
}

void UDDSAttributeSet::OnRep_PhysicalASR(const FGameplayAttributeData& OldPhysicalASR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, PhysicalASR, OldPhysicalASR);
}

void UDDSAttributeSet::OnRep_DexterityASR(const FGameplayAttributeData& OldDexterityASR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DexterityASR, OldDexterityASR);
}

void UDDSAttributeSet::OnRep_MagicASR(const FGameplayAttributeData& OldMagicASR) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MagicASR, OldMagicASR);
}

void UDDSAttributeSet::OnRep_AttackRequireStamina(const FGameplayAttributeData& OldAttackRequireStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, AttackRequireStamina, OldAttackRequireStamina);
}

void UDDSAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DamageTaken, OldDamageTaken);
}

void UDDSAttributeSet::PrintAllAttributes() const
{
	MY_LOG(LogTemp, Log, TEXT("---- Attributes of %s ----"), *GetName());
	MY_LOG(LogTemp, Log, TEXT("Level: %f"), GetLevel());
	MY_LOG(LogTemp, Log, TEXT("Energy: %f"), GetEnergy());
	MY_LOG(LogTemp, Log, TEXT("RequireEnergy: %f"), GetRequireEnergy());
	MY_LOG(LogTemp, Log, TEXT("Soul: %f"), GetSoul());
	
	MY_LOG(LogTemp, Log, TEXT("Vitality: %f"), GetVitality());
	MY_LOG(LogTemp, Log, TEXT("Endurance: %f"), GetEndurance());
	MY_LOG(LogTemp, Log, TEXT("Strength: %f"), GetStrength());
	MY_LOG(LogTemp, Log, TEXT("Dexterity: %f"), GetDexterity());
	MY_LOG(LogTemp, Log, TEXT("Magic: %f"), GetMagic());
	
	MY_LOG(LogTemp, Log, TEXT("Health: %f / %f"), GetHealth(), GetHealthMax());
	MY_LOG(LogTemp, Log, TEXT("Stamina: %f / %f"), GetStamina(), GetStaminaMax());
	MY_LOG(LogTemp, Log, TEXT("Mana: %f / %f"), GetMana(), GetManaMax());

	MY_LOG(LogTemp, Log, TEXT("AttackRequireStamina: %f"), GetAttackRequireStamina());
	
	MY_LOG(LogTemp, Log, TEXT("StrengthAR: %f"), GetStrengthAR());
	MY_LOG(LogTemp, Log, TEXT("StrengthARPlus: %f"), GetStrengthARPlus());
	MY_LOG(LogTemp, Log, TEXT("DexterityAR: %f"), GetDexterityAR());
	MY_LOG(LogTemp, Log, TEXT("DexterityARPlus: %f"), GetDexterityARPlus());
	MY_LOG(LogTemp, Log, TEXT("MagicAR: %f"), GetMagicAR());
	MY_LOG(LogTemp, Log, TEXT("MagicARPlus: %f"), GetMagicARPlus());

	MY_LOG(LogTemp, Log, TEXT("PhysicalASR: %f"), GetPhysicalASR());
	MY_LOG(LogTemp, Log, TEXT("DexterityASR: %f"), GetDexterityASR());
	MY_LOG(LogTemp, Log, TEXT("MagicASR: %f"), GetMagicASR());
	
	
	MY_LOG(LogTemp, Log, TEXT("AttackPower: %f"), GetAttackPower());
	MY_LOG(LogTemp, Log, TEXT("MagicPower: %f"), GetMagicPower());
	
	MY_LOG(LogTemp, Log, TEXT("PhysicalDefense: %f"), GetPhysicalDefense());
	MY_LOG(LogTemp, Log, TEXT("MagicDefense: %f"), GetMagicDefense());
	MY_LOG(LogTemp, Log, TEXT("PhysicalResist: %f"), GetPhysicalResist());
	MY_LOG(LogTemp, Log, TEXT("MagicResist: %f"), GetMagicResist());
	
	MY_LOG(LogTemp, Log, TEXT("DamageTaken: %f"), GetDamageTaken());
}

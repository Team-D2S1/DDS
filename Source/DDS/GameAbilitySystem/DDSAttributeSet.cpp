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
    InitDexterity(10.0f);
    InitMagic(10.0f);
    
    InitHealth(100.0f);
    InitHealthMax(100.0f);
    InitStamina(100.0f);
    InitStaminaMax(100.0f);
    InitMana(100.0f);
    InitManaMax(100.0f);
    
    InitAttackPower(10.0f);
    InitMagicPower(10.0f);
    
    InitPhysicalDefense(5.0f);
    InitMagicDefense(5.0f);
    InitPhysicalResist(0.0f);
    InitFireResist(0.0f);
    
    InitDamageTaken(0.0f);
 
	using namespace DDSGameplayTags;
    
    TagToAttributeMap.Add(Attribute_Default_Level, GetLevelAttribute);
    TagToAttributeMap.Add(Attribute_Default_Energy, GetEnergyAttribute);
    TagToAttributeMap.Add(Attribute_Default_RequireEnergy, GetRequireEnergyAttribute);
    TagToAttributeMap.Add(Attribute_Default_Soul, GetSoulAttribute);
    
    TagToAttributeMap.Add(Attribute_Primary_Vitality, GetVitalityAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Endurance, GetEnduranceAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Dexterity, GetDexterityAttribute);
    TagToAttributeMap.Add(Attribute_Primary_Magic, GetMagicAttribute);
    
    TagToAttributeMap.Add(Attribute_Status_Health, GetHealthAttribute);
    TagToAttributeMap.Add(Attribute_Status_Stamina, GetStaminaAttribute);
    TagToAttributeMap.Add(Attribute_Status_Mana, GetManaAttribute);
    TagToAttributeMap.Add(Attribute_Status_HealthMax, GetHealthMaxAttribute);
    TagToAttributeMap.Add(Attribute_Status_StaminaMax, GetStaminaMaxAttribute);
    TagToAttributeMap.Add(Attribute_Status_ManaMax, GetManaMaxAttribute);
    
    TagToAttributeMap.Add(Attribute_Offense_AttackPower, GetAttackPowerAttribute);
    TagToAttributeMap.Add(Attribute_Offense_MagicPower, GetMagicPowerAttribute);
    
    TagToAttributeMap.Add(Attribute_Defense_PhysicalDefense, GetPhysicalDefenseAttribute);
    TagToAttributeMap.Add(Attribute_Defense_MagicDefense, GetMagicDefenseAttribute);
    TagToAttributeMap.Add(Attribute_Defense_PhysicalResist, GetPhysicalResistAttribute);
    TagToAttributeMap.Add(Attribute_Defense_FireResist, GetFireResistAttribute);
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
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Magic, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, StaminaMax, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
    
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MagicDefense, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, PhysicalResist, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, FireResist, COND_None, REPNOTIFY_Always);

}

void UDDSAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	if (!CachedPawnUIInterface.IsValid())
	{
		MY_ERROR_DISPLAY(TEXT("PawnUIInterface is not implemented"));
		return;
	}
	MY_LOG(LogTemp, Log, TEXT("PostGameplayEffectExecute called for %s"), *GetName());

	bool hasAuthority = GetOwningAbilitySystemComponent()->GetOwner()->HasAuthority();
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	if (!PawnUIComponent)
	{
		MY_ERROR_DISPLAY(TEXT("Couldn't get PawnUIComponent from %s"), *GetName());
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = FMath::Clamp( GetHealth(), 0.f, GetHealthMax());
		MY_CLOG_DISPLAY_NET(FColor::Green, hasAuthority, TEXT("%s Health changed from %f to %f"), *GetName(), GetHealth(), NewHealth);
		SetHealth(NewHealth);
		if (hasAuthority)
		{
			PawnUIComponent->Multicast_OnHealthChanged(NewHealth, GetHealthMax());
		}
		else
		{
			PawnUIComponent->OnHealthChanged.Broadcast(NewHealth);
		}
		// PawnUIComponent->OnHealthChanged.Broadcast(NewHealth);
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float NewStamina = FMath::Clamp( GetStamina(), 0.f, GetStaminaMax());
		SetStamina(NewStamina);

		UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnStaminaChanged.Broadcast(NewStamina);
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("Couldn't get PlayerUIComponent from %s"), *GetName());
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaMaxAttribute())
	{
		const float NewMaxStamina = FMath::Clamp( GetStaminaMax(), 0.f, GetStaminaMax());
		SetStaminaMax(NewMaxStamina);

		UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			PlayerUIComponent->Multicast_OnMaxStaminaChanged(NewMaxStamina, GetStaminaMax());
		}
		else
		{
			MY_ERROR_DISPLAY(TEXT("Couldn't get PlayerUIComponent from %s"), *GetName());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetHealth();
		const float TakenDamage = GetDamageTaken();
		const float NewHealth = FMath::Clamp(OldHealth - TakenDamage, 0.f, GetHealthMax());
		SetHealth(NewHealth);
		if (hasAuthority)
		{
			PawnUIComponent->Multicast_OnHealthChanged(NewHealth, OldHealth);
		}
		else
		{
			PawnUIComponent->OnHealthChanged.Broadcast(NewHealth);
		}

		bool authority = GetOwningAbilitySystemComponent()->GetOwner()->HasAuthority();
		MY_CLOG_DISPLAY_NET(FColor::Red,authority, TEXT("OldHealth: %f, TakenDamage: %f, NewHealth: %f"), OldHealth, TakenDamage, NewHealth);

		if (GetHealth() <= 0.f)
		{
			if(UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead.Start"));
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthMaxAttribute())
	{
		const float NewMaxHealth = FMath::Clamp( GetHealthMax(), 0.f, GetHealthMax());
		SetHealthMax(NewMaxHealth);
		if (hasAuthority)
		{
			PawnUIComponent->Multicast_OnMaxHealthChanged(NewMaxHealth, GetHealthMax());
		}
		else
		{
			PawnUIComponent->OnMaxHealthChanged.Broadcast(NewMaxHealth);
		}
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

void UDDSAttributeSet::OnRep_FireResist(const FGameplayAttributeData& OldFireResist) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, FireResist, OldFireResist);
}

void UDDSAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DamageTaken, OldDamageTaken);
}
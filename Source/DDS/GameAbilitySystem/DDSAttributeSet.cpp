// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include  "GameplayEffectExtension.h"
#include "Components/UI/PawnUIComponent.h"
#include "ETC/CustomLog.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PlayerUIComponent.h"

UDDSAttributeSet::UDDSAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);

	InitStamina(50.f);
	InitMaxStamina(100.f);

	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UDDSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 각 GAD를 Replicated 되도록 등록
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, DefensePower, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UDDSAttributeSet, DamageTaken, COND_None, REPNOTIFY_Always);
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
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	if (!PawnUIComponent)
	{
		MY_ERROR_DISPLAY(TEXT("Couldn't get PawnUIComponent from %s"), *GetName());
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = FMath::Clamp( GetHealth(), 0.f, GetMaxHealth());
		SetHealth(NewHealth);
		PawnUIComponent->OnHealthChanged.Broadcast(NewHealth, GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float NewStamina = FMath::Clamp( GetStamina(), 0.f, GetMaxStamina());
		SetStamina(NewStamina);

		UPlayerUIComponent* PlayerUIComponent = CachedPawnUIInterface->GetPlayerUIComponent();
		if (PlayerUIComponent)
		{
			PlayerUIComponent->OnStaminaChanged.Broadcast(NewStamina, GetMaxStamina());
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
		const float NewHealth = FMath::Clamp(OldHealth - TakenDamage, 0.f, GetMaxHealth());
		SetHealth(NewHealth);
		PawnUIComponent->OnHealthChanged.Broadcast(NewHealth, GetMaxHealth());

		// TODO : UI 알리기
		bool authority = GetOwningAbilitySystemComponent()->GetOwner()->HasAuthority();
		MY_CLOG_DISPLAY_NET(FColor::Red,authority, TEXT("OldHealth: %f, TakenDamage: %f, NewHealth: %f"), OldHealth, TakenDamage, NewHealth);

		if (GetHealth() <= 0.f)
		{
			// TODO : 죽음 처리
		}
	}

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		const float NewMaxHealth = FMath::Clamp( GetMaxHealth(), 0.f, GetMaxHealth());
		SetMaxHealth(NewMaxHealth);
		PawnUIComponent->OnHealthChanged.Broadcast(NewMaxHealth, GetMaxHealth());
	}
}


/**
 * @brief 데이터가 서버에서 클라이언트로 전달될 때 호출됨.
 */
void UDDSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// AbilitySystem에게 알려주기
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Health, OldHealth);
}

void UDDSAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MaxHealth, OldMaxHealth);
}

void UDDSAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, Stamina, OldStamina);
}

void UDDSAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, MaxStamina, OldMaxStamina);
}

void UDDSAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, AttackPower, OldAttackPower);
}

void UDDSAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DefensePower, OldDefensePower);
}

void UDDSAttributeSet::OnRep_DamageTaken(const FGameplayAttributeData& OldDamageTaken) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDDSAttributeSet, DamageTaken, OldDamageTaken);
}
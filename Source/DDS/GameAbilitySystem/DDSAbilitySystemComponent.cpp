// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "DDSAbilitySystemComponent.h"

#include "DDSGameplayTags.h"
#include "Abilities/DDSGameplayAbility.h"
#include "DDSTypes/DDSStructTypes.h"
#include "ETC/CustomLog.h"
#include "GameAbilitySystem/DDSAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"

void UDDSAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDDSAbilitySystemComponent, LastDodgeInputDirection);
}

void UDDSAbilitySystemComponent::AbilityActorInfoSet()
{
	// Effect 적용시 서버에서 호출되는 이벤트 델리게이트
	// OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDDSAbilitySystemComponent::OnGameplayEffectAppliedToSelf);
	// SetIsReplicated(true);
}

void UDDSAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UDDSGameplayAbility>>& StartupAbilities)
{
	// MY_LOG_DISPLAY("%s AddCharacterAbilities",*GetOwner()->GetName());
	MY_LOG(LogTemp,Type::Log,"%s AddCharacterAbilities",*GetOwner()->GetName());
	for (const TSubclassOf<UDDSGameplayAbility>& Ability : StartupAbilities)
	{
		if (!Ability)
			continue;
		
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = 1;
		MY_LOG(LogTemp,Type::Log,"Granting %s",*Ability->GetName());
		// TSubclassOf에서 UDDSGameplayAbility 가져오기
		// UDDSGameplayAbility* UDDSGameplayAbility = Ability.GetDefaultObject();
		// // Ongiven처리
		// if (UDDSGameplayAbility->GetAbilityActivationPolicy() == EDDSAbilityActivationPolicy::OnGiven)
		// {
		// 	
		// 	GiveAbilityAndActivateOnce(AbilitySpec);
		// }else
		{
			GiveAbility(AbilitySpec);
		}

	}
}

void UDDSAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	for (const FGameplayAbilitySpec& spec : GetActivatableAbilities())
	{
		if (spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			// spec 이름 출력
			bool isServer = GetOwner()->HasAuthority();
			// MY_CLOG_DISPLAY_NET(FColor::Emerald,isServer,TEXT("Try to Activate Ability : %s "),*spec.Ability->GetName());
			MY_LOG(LogTemp,Type::Log,"Try to Activate Ability : %s ",*spec.Ability->GetName());
			TryActivateAbility(spec.Handle);
		}
	}
}


void UDDSAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

void UDDSAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FDDSPlayerAbilitySet>& InDefaultWeaponAbilities,
	int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		MY_LOG_DISPLAY("InDefaultWeaponAbilities is Empty");
		return;
	}
	for (const FDDSPlayerAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
			continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		AbilitySpec.Level = ApplyLevel;
		OutHandles.AddUnique( GiveAbility(AbilitySpec));
		// MY_LOG(LogTemp, Log, TEXT("Granting %s, tag: %s"), *AbilitySet.AbilityToGrant->GetName(), *AbilitySet.InputTag.ToString());
		bool isServer = GetOwner()->HasAuthority();
		MY_LOG_DISPLAY_NET(isServer, TEXT("Granting %s, tag: %s"), *AbilitySet.AbilityToGrant->GetName(), *AbilitySet.InputTag.ToString());
	}
}

void UDDSAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(TArray<FGameplayAbilitySpecHandle>& InHandles)
{
	for (const FGameplayAbilitySpecHandle& Handle : InHandles)
		{
			if (Handle.IsValid())
			{
				ClearAbility(Handle);
			}
		}
	InHandles.Empty();
}
void UDDSAbilitySystemComponent::Multicast_AddLooseGameplayTag_Implementation(const FGameplayTag& InTag)
{
	bool isServer = GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::White,isServer,TEXT("AddLooseGameplayTag : %s"),*InTag.ToString());
	AddLooseGameplayTag(InTag);
}

void UDDSAbilitySystemComponent::Multicast_RemoveLooseGameplayTag_Implementation(const FGameplayTag& InTag)
{
	bool isServer = GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::White,isServer,TEXT("RemoveLooseGameplayTag : %s"),*InTag.ToString());
	RemoveLooseGameplayTag(InTag);
}

void UDDSAbilitySystemComponent::BindAttributeValueChangeDelegates(UDDSAttributeSet* InAttributeSet)
{
	if (!InAttributeSet)
	{
		return;
	}

	// Level / Health / HealthMax / DamageTaken 등 Attribute 값 변경을 Listen
	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetLevelAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleLevelChanged);
	
	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetHealthAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleHealthChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetStaminaAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleStaminaChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetManaAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleManaChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetHealthMaxAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleHealthMaxChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetStaminaMaxAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleStaminaMaxChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetManaMaxAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleManaMaxChanged);

	GetGameplayAttributeValueChangeDelegate(InAttributeSet->GetDamageTakenAttribute())
		.AddUObject(this, &UDDSAbilitySystemComponent::HandleDamageTakenChanged);
}

void UDDSAbilitySystemComponent::Server_UseAttributePointToAttribute_Implementation(const FGameplayTag& InAttributeTag)
{
	if (!InAttributeTag.IsValid())
	{
		MY_LOG_DISPLAY("InAttributeTag is not valid");
		return;
	}

	if (!UseAttributePointEffectClass)
	{
		MY_LOG_DISPLAY("UseAttributePointEffectClass is not set");
		return;
	}

	// 만약 포인트가 0 이하라면 반환
	const UDDSAttributeSet* AttributeSet = GetSet<UDDSAttributeSet>();
	if (!AttributeSet)
	{
		MY_LOG_DISPLAY("AttributeSet is not valid");
		return;
	}
	if (AttributeSet->GetAttributePoints() <= 0.f)
	{
		MY_LOG_DISPLAY("Not enough AttributePoint to use");
		return;
	}

	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(UseAttributePointEffectClass, 1.f, EffectContext);
	if (!SpecHandle.IsValid())
	{
		MY_LOG_DISPLAY("SpecHandle is not valid");
		return;
	}

	// 태그 매개변수를 설정 (SetByCaller 사용)
	SpecHandle.Data->SetSetByCallerMagnitude(InAttributeTag, 1.f);

	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	MY_CLOG_DISPLAY_NET(FColor::Cyan, bIsServer, TEXT("Used AttributePoint for: %s"), *InAttributeTag.ToString());

	// 기본 속성이 변경되었으므로 PlayerStats Effect를 Refresh하여 파생 속성 재계산
	if (PlayerStatsEffectClass)
	{
		ApplyOrRefreshGameplayEffectToSelf(PlayerStatsEffectClass, 1.f, DDSGameplayTags::GameplayEffect_PlayerStats);
		MY_CLOG_DISPLAY_NET(FColor::Green, bIsServer, TEXT("Refreshed PlayerStats Effect to recalculate derived attributes"));
	}
}

void UDDSAbilitySystemComponent::LevelUp(int32 LevelsToAdd)
{
	// 서버에서만 실행
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		MY_LOG_DISPLAY("LevelUp can only be called on the server");
		return;
	}

	if (LevelsToAdd <= 0)
	{
		MY_LOG_DISPLAY("LevelsToAdd must be greater than 0");
		return;
	}

	const UAttributeSet* AttributeSetConst = GetAttributeSet(UDDSAttributeSet::StaticClass());
	UDDSAttributeSet* AS = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AttributeSetConst));
	if (!AS)
	{
		MY_LOG_DISPLAY("AttributeSet not found");
		return;
	}

	const int32 CurrentLevel = FMath::RoundToInt(AS->GetLevel());
	const int32 NewLevel = CurrentLevel + LevelsToAdd;

	// Level을 직접 설정
	AS->SetLevel(NewLevel);

	bool bIsServer = GetOwner()->HasAuthority();
	MY_CLOG_DISPLAY_NET(FColor::Yellow, bIsServer, 
		TEXT("Level Up! %d -> %d (+%d levels)"), 
		CurrentLevel, NewLevel, LevelsToAdd);
}

void UDDSAbilitySystemComponent::HandleLevelChanged(const FOnAttributeChangeData& Data)
{
	const int32 NewLevel = FMath::RoundToInt(Data.NewValue);
	const int32 OldLevel = FMath::RoundToInt(Data.OldValue);
	
	OnLevelChanged.Broadcast(NewLevel);

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	MY_CLOG_DISPLAY_NET(FColor::Yellow, bIsServer, 
		TEXT("[ASC] Level Changed: %d -> %d"), OldLevel, NewLevel);
}

void UDDSAbilitySystemComponent::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);

	// 디버그 메시지
	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Green, bIsServer, TEXT("[ASC] Health Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

void UDDSAbilitySystemComponent::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged.Broadcast(Data.NewValue);

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	bool bIsReduce = Data.NewValue < Data.OldValue;
	if (bIsReduce)
	{
		// MY_CLOG_DISPLAY_NET(FColor::Orange, bIsServer, TEXT("[ASC] Stamina Reduced: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
	}
}

void UDDSAbilitySystemComponent::HandleManaChanged(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);

	// bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Cyan, bIsServer, TEXT("[ASC] Mana Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

void UDDSAbilitySystemComponent::HandleHealthMaxChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);

	// bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Emerald, bIsServer, TEXT("[ASC] HealthMax Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

void UDDSAbilitySystemComponent::HandleStaminaMaxChanged(const FOnAttributeChangeData& Data)
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Silver, bIsServer, TEXT("[ASC] StaminaMax Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

void UDDSAbilitySystemComponent::HandleManaMaxChanged(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Blue, bIsServer, TEXT("[ASC] ManaMax Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

void UDDSAbilitySystemComponent::HandleDamageTakenChanged(const FOnAttributeChangeData& Data)
{
	OnDamageTakenChanged.Broadcast(Data.NewValue);

	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	// MY_CLOG_DISPLAY_NET(FColor::Red, bIsServer, TEXT("[ASC] DamageTaken Changed: %.2f -> %.2f"), Data.OldValue, Data.NewValue);
}

FActiveGameplayEffectHandle UDDSAbilitySystemComponent::ApplyOrRefreshGameplayEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level, const FGameplayTag& TagToMatch)
{
    if (!EffectClass)
    {
        MY_LOG(LogTemp, Error, TEXT("ApplyOrRefreshGameplayEffectToSelf called with null EffectClass"));
        return FActiveGameplayEffectHandle();
    }

    // Collect active handles to remove
    TArray<FActiveGameplayEffectHandle> HandlesToRemove;

    if (TagToMatch.IsValid())
    {
        FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(TagToMatch));
        HandlesToRemove = GetActiveEffects(Query);
    }
    else
    {
        // If no tag provided, we don't attempt to find matching effects here. The caller should provide a tag
        // for reliable refresh behavior. Just apply the effect.
        MY_LOG(LogTemp, Warning, TEXT("ApplyOrRefreshGameplayEffectToSelf called without TagToMatch; will simply apply effect without refreshing existing ones."));
    }

    // Remove collected handles
    for (const FActiveGameplayEffectHandle& H : HandlesToRemove)
    {
        if (H.IsValid())
        {
            RemoveActiveGameplayEffect(H);
        }
    }

    // Apply new effect
    FGameplayEffectContextHandle EffectContext = MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(EffectClass, Level, EffectContext);
    if (!SpecHandle.IsValid())
    {
        MY_LOG(LogTemp, Error, TEXT("Failed to make outgoing spec for effect %s"), *EffectClass->GetName());
        return FActiveGameplayEffectHandle();
    }

    return ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UDDSAbilitySystemComponent::SetLastDodgeInputDirection(const FVector& InDirection)
{
	LastDodgeInputDirection = InDirection;
	
	#if WITH_EDITOR
	bool bIsServer = GetOwner() && GetOwner()->HasAuthority();
	MY_CLOG_DISPLAY_NET(FColor::Cyan, bIsServer, TEXT("SetLastDodgeInputDirection: %s"), *InDirection.ToString());
	#endif
}


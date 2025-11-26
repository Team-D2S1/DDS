// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MonsterBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/WidgetComponent.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "Components/UI/MonsterUIComponent.h"
#include "Components/Utils/PatrolComponent.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"
#include "Engine/AssetManager.h"
#include "ETC/CustomLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/DDSUserWidget.h"


AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // AI 컨트롤러를 자동으로 할당

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f; // 안움직일때 감속
	GetCharacterMovement()->bUseRVOAvoidance;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 50.f;

	PatrolComponent = CreateDefaultSubobject<UPatrolComponent>(TEXT("Patrol Component"));
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh());
	
	AbilitySystemComponent = CreateDefaultSubobject<UDDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDDSAttributeSet>(TEXT("AttributeSet"));
	
	MonsterCombatComponent = CreateDefaultSubobject<UMonsterCombatComponent>(TEXT("MonsterCombatComponent"));
	MonsterCombatComponent->SetIsReplicated(true);

	MonsterUIComponent = CreateDefaultSubobject<UMonsterUIComponent>(TEXT("MonsterUIComponent"));
	MonsterUIComponent->SetIsReplicated(true);

	MonsterHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterHealthWidgetComponent"));
	MonsterHealthWidgetComponent->SetIsReplicated(true);
	MonsterHealthWidgetComponent->SetupAttachment(GetMesh());
}

bool AMonsterBase::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining,
	float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	
	return false;
}

UPawnCombatComponent* AMonsterBase::GetCombatComponent() const
{
	return MonsterCombatComponent;
}

UPawnUIComponent* AMonsterBase::GetPawnUIComponent() const
{
	return MonsterUIComponent;
}

UMonsterUIComponent* AMonsterBase::GetMonsterUIComponent() const
{
	return MonsterUIComponent;
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket"));
	
	if (!AbilitySystemComponent)
	{
		MY_ERROR_DISPLAY(TEXT("AbilitySystemComponent is null"));
		return;
	}
	
	// AttributeSet이 null이면 동적으로 생성 (블루프린트에서 덮어씌워진 경우 대응)
	if (!AttributeSet)
	{
		bool hasAuthority = HasAuthority();
		UE_LOG(LogTemp, Warning, TEXT("[%s] [%s::BeginPlay] AttributeSet is null! Creating new one. This might be caused by BP overriding."), 
			hasAuthority ? TEXT("Server") : TEXT("Client"),
			*GetName());
		
		UDDSAttributeSet* NewAttributeSet = NewObject<UDDSAttributeSet>(this, UDDSAttributeSet::StaticClass());
		if (NewAttributeSet)
		{
			// ASC에 AttributeSet 추가 후 반환값을 AttributeSet에 할당
			const UAttributeSet* AddedSet = AbilitySystemComponent->AddAttributeSetSubobject(NewAttributeSet);
			AttributeSet = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AddedSet));
			UE_LOG(LogTemp, Log, TEXT("[%s] [%s::BeginPlay] Successfully created and added AttributeSet"), 
				hasAuthority ? TEXT("Server") : TEXT("Client"),
				*GetName());
		}
	}
	
	bool hasAuthority = HasAuthority();
	UE_LOG(LogTemp, Log, TEXT("[%s] [%s::BeginPlay] AttributeSet=%s, ASC=%s"), 
		hasAuthority ? TEXT("Server") : TEXT("Client"),
		*GetName(),
		AttributeSet ? TEXT("Valid") : TEXT("NULL"),
		AbilitySystemComponent ? TEXT("Valid") : TEXT("NULL"));
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	// InitAbilityActorInfo 이후에도 AttributeSet이 여전히 null이면 다시 시도
	if (!AttributeSet)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] [%s::BeginPlay] AttributeSet is still NULL after InitAbilityActorInfo! Attempting recovery..."), 
			hasAuthority ? TEXT("Server") : TEXT("Client"),
			*GetName());
			
		// ASC에서 AttributeSet을 가져와 보기
		const UAttributeSet* FoundAttributeSet = AbilitySystemComponent->GetAttributeSet(UDDSAttributeSet::StaticClass());
		if (FoundAttributeSet)
		{
			AttributeSet = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(FoundAttributeSet));
			UE_LOG(LogTemp, Warning, TEXT("[%s] [%s::BeginPlay] Found AttributeSet from ASC"), 
				hasAuthority ? TEXT("Server") : TEXT("Client"),
				*GetName());
		}
		else
		{
			// 여전히 없으면 새로 생성
			UDDSAttributeSet* NewAttributeSet = NewObject<UDDSAttributeSet>(this, UDDSAttributeSet::StaticClass());
			if (NewAttributeSet)
			{
				const UAttributeSet* AddedSet = AbilitySystemComponent->AddAttributeSetSubobject(NewAttributeSet);
				AttributeSet = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(AddedSet));
				UE_LOG(LogTemp, Warning, TEXT("[%s] [%s::BeginPlay] Created new AttributeSet as last resort"), 
					hasAuthority ? TEXT("Server") : TEXT("Client"),
					*GetName());
			}
		}
	}
	if (GetNetMode() != NM_DedicatedServer) 
	{
		if (UDDSUserWidget* HealthWidget = Cast<UDDSUserWidget>(MonsterHealthWidgetComponent->GetUserWidgetObject()))
		{
			HealthWidget->InitMonsterCreatedWidget(this);
		}else
		{
			MY_LOG(LogTemp, Type::Warning, TEXT("MonsterHealthWidgetComponent is not a UDDSUserWidget"));
		}
	}
	MonsterUIComponent->BroadcastInitialValues(AttributeSet);
	MonsterUIComponent->OnPawnInitializingFinished.Broadcast();
	AbilitySystemComponent->BindAttributeValueChangeDelegates(AttributeSet);
	MY_LOG(LogTemp, Type::Log, TEXT("MonsterBase BeginPlay Finished"));
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMonsterBase::OnDeathStartTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	Super::OnDeathStartTagChanged(ChangedTag, NumberOfTag);
	
	if(HasAuthority())
	{
		// BT 종료
		if(AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->BrainComponent->StopLogic(FString("Monster Death"));
		}

		// 모든 플레이어에게 경험치와 소울 지급
		GiveRewardToAllPlayers();

		NM_MonsterDie();
	}
}

void AMonsterBase::NM_MonsterDie_Implementation()
{
	MY_LOG(LogTemp, Error, TEXT("Monster Death"))

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		if(GetMonsterCombatComponent()->DeathMontage)
		{
			AnimInstance->Montage_Play(GetMonsterCombatComponent()->DeathMontage);
		}
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnDeathMontageEnded);
	}
}

void AMonsterBase::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if(!HasAuthority()) return;
	
	if(Montage == GetMonsterCombatComponent()->DeathMontage)
	{
		if(GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead.End"));
		}
	}
}

void AMonsterBase::OnDeathEndTagChanged(const FGameplayTag ChangedTag, int32 NumberOfTag)
{
	Super::OnDeathEndTagChanged(ChangedTag, NumberOfTag);
}



void AMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitMonsterStartUpData();
}

void AMonsterBase::OnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle handle)
{
}

void AMonsterBase::InitMonsterStartUpData()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if(ASC)
	{
		ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnGameplayEffectApplied);
	}
	
	if (EntityStartUpDataBase.IsNull())
	{
		return;
	}
	
	// 몬스터는 그 수가 많아 게임을 멈출 수 있음. => 비동기 로딩 사용
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		EntityStartUpDataBase.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
		{
			UDataAsset_StartUpDataBase* loadedData = EntityStartUpDataBase.Get();
			if (!loadedData)
			{
				return;
			}
			loadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
		})
	);
}

void AMonsterBase::OnFocus()
{
	bIsFocused = true;
	DrawDebugSphere(GetWorld(), GetActorLocation(), 100.0f, 12, FColor::Red, false, 0.2f);
}

void AMonsterBase::OnFocusLost()
{
	bIsFocused = false;
}

void AMonsterBase::GiveRewardToAllPlayers()
{
	if (!HasAuthority())
	{
		return;
	}

	// 몬스터의 AttributeSet에서 보상 정보 가져오기
	if (!AttributeSet)
	{
		MY_LOG(LogTemp, Warning, TEXT("[GiveRewardToAllPlayers] Monster has no AttributeSet"));
		return;
	}

	const float ExperienceDrop = AttributeSet->GetEnergy();
	const float SoulDrop = AttributeSet->GetSoul();

	if (ExperienceDrop <= 0.f && SoulDrop <= 0.f)
	{
		return;
	}

	// 월드의 모든 PlayerController 찾기
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	int32 RewardedPlayerCount = 0;
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (!PC)
		{
			continue;
		}

		APawn* PlayerPawn = PC->GetPawn();
		if (!PlayerPawn)
		{
			continue;
		}

		// 플레이어의 AbilitySystemComponent 가져오기
		UAbilitySystemComponent* PlayerASC = nullptr;
		if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PlayerPawn))
		{
			PlayerASC = ASI->GetAbilitySystemComponent();
		}

		if (!PlayerASC)
		{
			continue;
		}

		// 플레이어 AttributeSet 가져오기
		const UAttributeSet* PlayerAttributeSetConst = PlayerASC->GetAttributeSet(UDDSAttributeSet::StaticClass());
		UDDSAttributeSet* PlayerAttributeSet = const_cast<UDDSAttributeSet*>(Cast<UDDSAttributeSet>(PlayerAttributeSetConst));

		if (!PlayerAttributeSet)
		{
			continue;
		}

		// 경험치 지급 (ASC를 통해 레벨업 체크까지 자동 수행)
		if (UDDSAbilitySystemComponent* DDSPlayerASC = Cast<UDDSAbilitySystemComponent>(PlayerASC))
		{
			if (ExperienceDrop > 0.f)
			{
				DDSPlayerASC->AddExperienceAndCheckLevelUp(ExperienceDrop);
			}
		}

		// 소울 지급
		if (SoulDrop > 0.f)
		{
			const float OldSoul = PlayerAttributeSet->GetSoul();
			PlayerAttributeSet->SetSoul(OldSoul + SoulDrop);
		}

		RewardedPlayerCount++;
	}

	// 로그 출력
	if (RewardedPlayerCount > 0)
	{
		MY_CLOG_DISPLAY_NET(FColor::Green, true, 
			TEXT("💰 Monster [%s] gave rewards to %d player(s): %.0f EXP, %.0f Soul"), 
			*GetName(), RewardedPlayerCount, ExperienceDrop, SoulDrop);
	}
}


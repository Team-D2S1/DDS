// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MonsterBase.h"

#include "Components/WidgetComponent.h"
#include "Components/Combat/MonsterCombatComponent.h"
#include "Components/UI/MonsterUIComponent.h"
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


	AbilitySystemComponent = CreateDefaultSubobject<UDDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);//
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

void AMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AMonsterBase, MonsterCombatComponent);
}


void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
	{
		MY_ERROR_DISPLAY(TEXT("AbilitySystemComponent is null"));
		return;
	}
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
	MY_LOG(LogTemp, Type::Log, TEXT("MonsterBase BeginPlay Finished"));
}

void AMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// TODO 테스트용, 삭제해야함
	if (bIsFocused)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 100.0f, 12, FColor::Red, false, 0.2f);
	}
}

void AMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Debug::Log(TEXT("AMonsterBase::PossessedBy"));
	InitMonsterStartUpData();
}
void AMonsterBase::InitMonsterStartUpData()
{
	if (EntityStartUpDataBase.IsNull())
	{
		return;
	}


	// Debug::Log(TEXT("AMonsterBase::InitMonsterStartUpData"));
	// 몬스터는 그 수가 많아 게임을 멈출 수 있음. => 비동기 로딩 사용
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		EntityStartUpDataBase.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
		{
			// Debug::Log(TEXT("AMonsterBase::InitMonsterStartUpData AsyncLoad"));
			UDataAsset_StartUpDataBase* loadedData = EntityStartUpDataBase.Get();
			if (!loadedData)
			{
				return;
			}
			// Debug::Log(TEXT("AMonsterBase::InitMonsterStartUpData loadedData"));
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

// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "MonsterBase.h"

#include "Components/Combat/MonsterCombatComponent.h"
#include "DataAsset/StartUpData/DataAsset_StartUpDataBase.h"
#include "DDS/GameAbilitySystem/DDSAbilitySystemComponent.h"
#include "DDS/GameAbilitySystem/DDSAttributeSet.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	MonsterCombatComponent = CreateDefaultSubobject<UMonsterCombatComponent>(TEXT("MonsterCombatComponent"));
	
	AbilitySystemComponent = CreateDefaultSubobject<UDDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);//
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDDSAttributeSet>(TEXT("AttributeSet"));
}



void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
		return;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
void AMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitMonsterStartUpData();
}
void AMonsterBase::InitMonsterStartUpData()
{
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

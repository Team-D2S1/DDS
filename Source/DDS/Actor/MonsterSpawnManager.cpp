// MonsterSpawnManager.cpp
#include "MonsterSpawnManager.h"

#include "AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "Character/Monster/MonsterBase.h"

AMonsterSpawnManager::AMonsterSpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMonsterSpawnManager::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoCollectMonstersInLevel)
    {
        CollectMonstersFromLevel();
    }
}

void AMonsterSpawnManager::CollectMonstersFromLevel()
{
    MonsterSpawnList.Empty();

    for (TSubclassOf<AActor> MonsterClass : MonsterClassesToCollect)
    {
        if (!MonsterClass) continue;

        // 레벨에 배치된 해당 클래스의 모든 액터 찾기
        for (TActorIterator<AActor> It(GetWorld(), MonsterClass); It; ++It)
        {
            AActor* Monster = *It;
            
            FMonsterSpawnInfo SpawnInfo;
            SpawnInfo.MonsterClass = MonsterClass;
            SpawnInfo.SpawnTransform = Monster->GetActorTransform();
            SpawnInfo.SpawnedMonster = Monster;

            MonsterSpawnList.Add(SpawnInfo);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Collected %d monsters from level"), MonsterSpawnList.Num());
}

void AMonsterSpawnManager::ResetAllMonsters()
{
    for(FMonsterSpawnInfo& SpawnInfo : MonsterSpawnList)
    {
        if(SpawnInfo.SpawnedMonster && IsValid(SpawnInfo.SpawnedMonster))
        {
            // 몬스터 체력 초기화
            AMonsterBase* Monster = Cast<AMonsterBase>(SpawnInfo.SpawnedMonster);
            if(Monster)
            {
                UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();

                FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
                EffectContext.AddSourceObject(Monster);

                FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HealEffect, 1.f, EffectContext);

                if(SpecHandle.IsValid())
                {
                    ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                }
            }
        }
    }
}

void AMonsterSpawnManager::DespawnAllMonsters()
{
    for (FMonsterSpawnInfo& SpawnInfo : MonsterSpawnList)
    {
        if (SpawnInfo.SpawnedMonster && IsValid(SpawnInfo.SpawnedMonster))
        {
            SpawnInfo.SpawnedMonster->Destroy();
            SpawnInfo.SpawnedMonster = nullptr;
        }
    }
}

void AMonsterSpawnManager::RespawnDeadMonsters()
{
    for (FMonsterSpawnInfo& SpawnInfo : MonsterSpawnList)
    {
        // 몬스터가 없거나 죽었으면 리스폰
        if (!SpawnInfo.SpawnedMonster || !IsValid(SpawnInfo.SpawnedMonster))
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AActor* NewMonster = GetWorld()->SpawnActor<AActor>(
                SpawnInfo.MonsterClass,
                SpawnInfo.SpawnTransform,
                SpawnParams
            );

            SpawnInfo.SpawnedMonster = NewMonster;
        }
    }
}
// MonsterSpawnManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;

	UPROPERTY()
	TObjectPtr<AActor> SpawnedMonster;
};

UCLASS()
class DDS_API AMonsterSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	AMonsterSpawnManager();

protected:
	virtual void BeginPlay() override;

	// 에디터에서 배치한 몬스터들 자동으로 등록
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<FMonsterSpawnInfo> MonsterSpawnList;

	// 레벨에 있는 몬스터들 자동 수집
	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bAutoCollectMonstersInLevel = true;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<AActor>> MonsterClassesToCollect;

public:
	// 모든 몬스터 체력 초기화
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ResetAllMonsters();

	// 모든 몬스터 제거
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void DespawnAllMonsters();

	// 죽은 몬스터들만 리스폰
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void RespawnDeadMonsters();

	// 레벨에 있는 몬스터 수집
	UFUNCTION(CallInEditor, Category = "Spawn")
	void CollectMonstersFromLevel();
};
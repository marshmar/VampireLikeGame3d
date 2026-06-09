#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Systems/Wave/PhaseData.h"
#include "SpawnManager.generated.h"

class AWaveTimer;

UCLASS()
class VAMPIRELIKE3D_API ASpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ASpawnManager();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    AWaveTimer* WaveTimer;

    UFUNCTION()
    void OnEnemyDied(TSubclassOf<ABaseEnemy> EnemyClass);

protected:
    virtual void BeginPlay() override;

private:
    FPhaseData CurrentPhaseData;
    int32 CurrentEnemyCount;
    int32 CurrentPhaseIndex;
    float nextStartTime;
    TArray<FPhaseData*> Rows;

    UPROPERTY(EditAnywhere)
    UDataTable* PhaseDataTable;

    // SpawnManager 헤더에 추가
    UPROPERTY(VisibleAnywhere)
    TMap<TSubclassOf<ABaseEnemy>, int32> SpawnedCountMap;

    void ChangePhase();
    void SpawnEnemy();
    FVector GetRandomSpawnLocation();
    TSubclassOf<ABaseEnemy> SelectEnemyClassByWeight();

    UFUNCTION()
    void OnPhaseChanged(const FPhaseData& NewPhaseData);
};
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
    void OnEnemyDied();

protected:
    virtual void BeginPlay() override;

private:
    FPhaseData CurrentPhaseData;
    int32 CurrentEnemyCount;

    void SpawnEnemy();
    FVector GetRandomSpawnLocation();
    TSubclassOf<ABaseEnemy> SelectEnemyClassByWeight();

    UFUNCTION()
    void OnPhaseChanged(const FPhaseData& NewPhaseData);
};
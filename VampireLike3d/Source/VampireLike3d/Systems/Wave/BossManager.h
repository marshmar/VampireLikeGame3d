#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossManager.generated.h"

// BossManager.h
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawned);

class ABossEnemy;
class AWaveTimer;

UCLASS()
class VAMPIRELIKE3D_API ABossManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<ABossEnemy> BossClass;

	UPROPERTY(EditAnywhere, Category = "Boss")
	AActor* BossSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Boss")
	float BossSpawnTime;

	UPROPERTY(EditAnywhere, Category = "Boss")
	AWaveTimer* WaveTimer;

	UPROPERTY()
	ABossEnemy* CurrentBoss;

	UPROPERTY(BlueprintAssignable)
	FOnBossDefeated OnBossDefeated;

	UPROPERTY(BlueprintAssignable)
	FOnBossSpawned OnBossSpawned;

	bool bIsBossSpawned;

	void SpawnBoss();

	UFUNCTION()
	void BroadcastBossDefeated();
	void DestroyBoss();

};

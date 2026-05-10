#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhaseData.generated.h"

class ABaseEnemy;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnWeight;
};

USTRUCT(BlueprintType)
struct FPhaseData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnInfo> EnemySpawnInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DifficultyMultiplier;
};

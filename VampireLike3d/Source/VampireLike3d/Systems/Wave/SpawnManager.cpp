// SpawnManager.cpp
#include "Systems/Wave/SpawnManager.h"
#include "Systems/Wave/WaveTimer.h"
#include "Systems/ObjectPool/PoolManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Entities/Enemies/BaseEnemy.h"

ASpawnManager::ASpawnManager()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentEnemyCount = 0;
}

void ASpawnManager::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(WaveTimer))
    {
        UE_LOG(LogTemp, Error, TEXT("WaveTimer is nullptr"));
        return;
    }

    // Load FirstPhase Data
    if (!IsValid(PhaseDataTable))
    {
        UE_LOG(LogTemp, Error, TEXT("PhaseDataTable is nullptr"));
        return;
    }

    PhaseDataTable->GetAllRows<FPhaseData>(TEXT(""), Rows);
    if (Rows.IsValidIndex(0))
    {
        CurrentPhaseData = *Rows[0];
        CurrentPhaseIndex = 0;
    }

    if (Rows.IsValidIndex(CurrentPhaseIndex + 1))
    {
        nextStartTime = Rows[CurrentPhaseIndex + 1]->StartTime;
    }

    // Initialize ObjectPool
    UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
    if (IsValid(PoolManager) && !CurrentPhaseData.EnemySpawnInfos.IsEmpty())
    {
        for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
        {
            PoolManager->InitializePoolByClass(Info.EnemyClass, CurrentPhaseData.TargetEnemyCount * Info.SpawnWeight);
        }
    }
}

void ASpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (nextStartTime!= -1.f && WaveTimer->GetElapsedTime() >= nextStartTime)
    {
        ChangePhase();
    }

    if (CurrentEnemyCount < CurrentPhaseData.TargetEnemyCount)
    {
        SpawnEnemy();
    }
}

void ASpawnManager::OnPhaseChanged(const FPhaseData& NewPhaseData)
{
    CurrentPhaseData = NewPhaseData;

    UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
    if (PoolManager)
    {
        for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
        {
            PoolManager->InitializePoolByClass(Info.EnemyClass, CurrentPhaseData.TargetEnemyCount);
        }
    }
}

void ASpawnManager::ChangePhase()
{
    int32 nextPhaseIndex = CurrentPhaseIndex + 1;
    if (!Rows.IsValidIndex(nextPhaseIndex))
    {
        UE_LOG(LogTemp, Error, TEXT("Next phase data does not exist."));
        nextStartTime = -1.f;
        return;
    }

    CurrentPhaseData = *Rows[nextPhaseIndex];
    CurrentPhaseIndex = nextPhaseIndex;

    // nextStartTime 업데이트도 누락되어 있었음!
    if (Rows.IsValidIndex(CurrentPhaseIndex + 1))
    {
        nextStartTime = Rows[CurrentPhaseIndex + 1]->StartTime;
    }
    else
    {
        nextStartTime = -1.f;
    }

    // 새 페이즈 풀 초기화
    UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
    if (PoolManager)
    {
        for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
        {
            PoolManager->InitializePoolByClass(Info.EnemyClass, CurrentPhaseData.TargetEnemyCount * Info.SpawnWeight);
        }
    }
}

void ASpawnManager::OnEnemyDied(TSubclassOf<ABaseEnemy> EnemyClass)
{
    if (SpawnedCountMap.Contains(EnemyClass))
    {
        SpawnedCountMap[EnemyClass]--;
    }
    CurrentEnemyCount = FMath::Max(0, CurrentEnemyCount - 1);
}

void ASpawnManager::SpawnEnemy()
{
    TSubclassOf<ABaseEnemy> EnemyClass = SelectEnemyClassByWeight();
    if (!EnemyClass) return;

    UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
    if (!PoolManager) return;

    FVector SpawnLocation = GetRandomSpawnLocation();
    FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

    AActor* Enemy = PoolManager->AcquireActorByClass(EnemyClass.Get(), SpawnTransform);
    if (IsValid(Enemy))
    {
        CurrentEnemyCount++;
        SpawnedCountMap.FindOrAdd(EnemyClass)++;

        ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(Enemy);
        if (BaseEnemy)
        {
            // 중복 방지
            BaseEnemy->OnEnemyDied.RemoveDynamic(this, &ASpawnManager::OnEnemyDied);
            BaseEnemy->OnEnemyDied.AddDynamic(this, &ASpawnManager::OnEnemyDied);
        }
    }
}

TSubclassOf<ABaseEnemy> ASpawnManager::SelectEnemyClassByWeight()
{
    if (CurrentPhaseData.EnemySpawnInfos.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Enemy Spawn info does not exist."));
        return nullptr;
    }

    float TotalWeight = 1.f;

    // 전체 소환된 수 합산
    int32 TotalSpawned = 0;
    for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
    {
        TotalSpawned += SpawnedCountMap.FindOrAdd(Info.EnemyClass); // 소환된 수 Map
    }

    // 목표 비율 대비 가장 부족한 클래스 선택
    TSubclassOf<ABaseEnemy> SelectedClass = nullptr;
    float MaxDeficit = -FLT_MAX;

    for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
    {
        float TargetRatio = Info.SpawnWeight / TotalWeight;
        float CurrentRatio = TotalSpawned > 0
            ? (float)SpawnedCountMap.FindRef(Info.EnemyClass)/ TotalSpawned
            : 0.f;

        float Deficit = TargetRatio - CurrentRatio; // 부족한 비율
        if (Deficit > MaxDeficit)
        {
            MaxDeficit = Deficit;
            SelectedClass = Info.EnemyClass;
        }
    }

    return SelectedClass;
}

FVector ASpawnManager::GetRandomSpawnLocation()
{
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player) return FVector::ZeroVector;

    float MinDist = 3000.f;
    float MaxDist = 4000.f;

    FVector RandomDir = UKismetMathLibrary::RandomUnitVector();
    RandomDir.Z = 0.f;
    RandomDir.Normalize();

    float RandomDist = FMath::FRandRange(MinDist, MaxDist);
    return Player->GetActorLocation() + RandomDir * RandomDist;
}
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

    if (WaveTimer)
    {
        UE_LOG(LogTemp, Warning, TEXT("WaveTimer 연결됨"));
        WaveTimer->OnPhaseChanged.AddDynamic(this, &ASpawnManager::OnPhaseChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("WaveTimer 없음"));
    }

    // 첫 페이즈 데이터 로드
    if (WaveTimer && WaveTimer->PhaseDataTable)
    {
        TArray<FPhaseData*> Rows;
        WaveTimer->PhaseDataTable->GetAllRows<FPhaseData>(TEXT(""), Rows);
        if (Rows.IsValidIndex(0))
        {
            CurrentPhaseData = *Rows[0];
        }
    }

    // 풀 초기화
    UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
    if (PoolManager && !CurrentPhaseData.EnemySpawnInfos.IsEmpty())
    {
        for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
        {
            PoolManager->InitializePoolByClass(Info.EnemyClass, CurrentPhaseData.TargetEnemyCount);
        }
    }
}

void ASpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

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

void ASpawnManager::OnEnemyDied()
{
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
    if (CurrentPhaseData.EnemySpawnInfos.IsEmpty()) return nullptr;

    float TotalWeight = 0.f;
    for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
    {
        TotalWeight += Info.SpawnWeight;
    }

    float RandValue = FMath::FRandRange(0.f, TotalWeight);
    float AccumulatedWeight = 0.f;

    for (const FEnemySpawnInfo& Info : CurrentPhaseData.EnemySpawnInfos)
    {
        AccumulatedWeight += Info.SpawnWeight;
        if (RandValue <= AccumulatedWeight)
        {
            return Info.EnemyClass;
        }
    }

    return CurrentPhaseData.EnemySpawnInfos[0].EnemyClass;
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
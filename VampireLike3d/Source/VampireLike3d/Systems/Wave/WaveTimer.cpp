#include "Systems/Wave/WaveTimer.h"

AWaveTimer::AWaveTimer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWaveTimer::BeginPlay()
{
	Super::BeginPlay();

	WaveDuration = 900.f;
	RemainingTime = WaveDuration;
	CurrentPhaseIndex = 0;
}

void AWaveTimer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RemainingTime = FMath::Max(0.f, RemainingTime - DeltaTime);
    CheckPhase();
}

float AWaveTimer::GetRemainingTime() const
{
	return this->RemainingTime;
}

void AWaveTimer::DecreaseRemainingTime(float Value)
{
	RemainingTime = FMath::Max(0.f, RemainingTime - Value);
}

void AWaveTimer::CheckPhase()
{
    if (!PhaseDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("PhaseDataTable is null"));
        return;
    }

    TArray<FPhaseData*> Rows;
    PhaseDataTable->GetAllRows<FPhaseData>(TEXT(""), Rows);

    if (!Rows.IsValidIndex(CurrentPhaseIndex + 1)) return;

    float ElapsedTime = WaveDuration - RemainingTime;
    if (ElapsedTime >= Rows[CurrentPhaseIndex + 1]->StartTime)
    {
        UE_LOG(LogTemp, Warning, TEXT("Phase Changed"));
        CurrentPhaseIndex++;
        OnPhaseChanged.Broadcast(*Rows[CurrentPhaseIndex]);
    }
}

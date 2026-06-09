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
}

void AWaveTimer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RemainingTime = FMath::Max(0.f, RemainingTime - DeltaTime);
	if (RemainingTime == 0)
	{
		PrimaryActorTick.bCanEverTick = false;
		EndTime();
	}
}

float AWaveTimer::GetRemainingTime() const
{
	return this->RemainingTime;
}

float AWaveTimer::GetElapsedTime() const
{
	return WaveDuration - RemainingTime;
}

void AWaveTimer::DecreaseRemainingTime(float Value)
{
	RemainingTime = FMath::Max(0.f, RemainingTime - Value);
}

void AWaveTimer::EndTime() const
{
	if (OnTimeEnded.IsBound())
	{
		OnTimeEnded.Broadcast();
	}
}

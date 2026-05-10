// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Wave/WaveTimerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "WaveTimer.h"


void UWaveTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    WaveTimer = Cast<AWaveTimer>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveTimer::StaticClass()));
}

void UWaveTimerWidget::UpdateTime(float RemainingTime)
{
    int32 Minutes = FMath::FloorToInt(RemainingTime / 60.f);
    int32 Seconds = FMath::FloorToInt(FMath::Fmod(RemainingTime, 60.f));

    FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
    RemainingTimeText->SetText(FText::FromString(TimeString));
}


void UWaveTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (WaveTimer)
    {
        UpdateTime(WaveTimer->GetRemainingTime());
    }
}
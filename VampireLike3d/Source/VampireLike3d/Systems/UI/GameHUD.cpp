// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Wave/WaveTimer.h"
#include "Systems/Wave/WaveTimerWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (WaveTimerWidgetClass != nullptr)
	{
		TimerWidget = CreateWidget<UWaveTimerWidget>(GetOwningPlayerController(), WaveTimerWidgetClass);
		TimerWidget->AddToViewport();

		AWaveTimer* WaveTimer = Cast<AWaveTimer>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveTimer::StaticClass()));
		TimerWidget->SetWaveTimer(WaveTimer);
	}
}

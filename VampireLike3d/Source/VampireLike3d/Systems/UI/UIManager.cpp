// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UI/UIManager.h"
#include "Systems/Wave/WaveTimerWidget.h"
#include "Kismet/GameplayStatics.h"

void UUIManager::InitializeTimerUI()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetGameInstance(), 0);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is nullptr"));
		return;
	}

	WaveTimerWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr,
		TEXT("/Game/BluePrints/Systems/Wave/WBP_WaveTimer.WBP_WaveTimer_C"));

	UE_LOG(LogTemp, Warning, TEXT("LoadedClass: %s"), WaveTimerWidgetClass ? *WaveTimerWidgetClass->GetName() : TEXT("nullptr"));
	if (!IsValid(WaveTimerWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveTimerWidgetClass is nullptr"));
		return;
	}

	WaveTimerWidget = CreateWidget<UWaveTimerWidget>(PlayerController, WaveTimerWidgetClass);
	if (IsValid(WaveTimerWidget))
	{
		WaveTimerWidget->AddToViewport();
	}
}

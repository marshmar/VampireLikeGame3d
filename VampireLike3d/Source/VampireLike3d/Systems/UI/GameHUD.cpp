// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Wave/WaveTimer.h"
#include "Systems/Wave/WaveTimerWidget.h"
#include "Systems/UI/HUDWidget.h"
#include "Systems/UI/UIManager.h"


void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    if (!UIManager) return;

    // 위젯 생성은 HUD에서, 관리는 UIManager에서
    APlayerController* Controller = UGameplayStatics::GetPlayerController(GetGameInstance(), 0);

    if (WaveTimerWidgetClass)
    {
        TimerWidget = CreateWidget<UWaveTimerWidget>(Controller, WaveTimerWidgetClass);
        if (TimerWidget)
        {
            TimerWidget->AddToViewport();
            UIManager->SetTimerWidget(TimerWidget);  // UIManager에 전달
        }
    }

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UHUDWidget>(Controller, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();
            UIManager->SetHUDWidget(HUDWidget);  // UIManager에 전달
            UIManager->SetBossHpBar(false);
        }
    }
}

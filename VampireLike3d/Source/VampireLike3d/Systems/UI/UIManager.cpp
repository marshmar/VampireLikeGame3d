// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/UI/UIManager.h"
#include "Systems/Wave/WaveTimerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/UI/HUDWidget.h"

void UUIManager::SetTimerWidget(UWaveTimerWidget* Widget)
{
	this->WaveTimerWidget = Widget;
}

void UUIManager::SetHUDWidget(UHUDWidget* Widget)
{
	this->HUDWidget = Widget;
}

void UUIManager::UpdateHpBar(float Current, float Max)
{
	if (IsValid(HUDWidget))
		HUDWidget->UpdateHpBar(Current, Max);
}

void UUIManager::UpdateBossHpBar(float Current, float Max)
{
	if (IsValid(HUDWidget))
		HUDWidget->UpdateBossHpBar(Current, Max);
}
void UUIManager::SetBossHpBar(bool bState)
{
	if (IsValid(HUDWidget))
		HUDWidget->SetBossHpBar(bState);
}

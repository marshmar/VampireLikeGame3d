// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UWaveTimerWidget;
class UHUDWidget;
/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// HUD에서 위젯 전달받기
	void SetTimerWidget(UWaveTimerWidget* Widget);
	void SetHUDWidget(UHUDWidget* Widget);

	// 플레이어에서 호출
	void UpdateHpBar(float Current, float Max);
	void UpdateBossHpBar(float Current, float Max);
	void SetBossHpBar(bool bState);
private:
	UPROPERTY()
	UWaveTimerWidget* WaveTimerWidget;

	UPROPERTY()
	UHUDWidget* HUDWidget;
};

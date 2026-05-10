// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UWaveTimerWidget;
/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeTimerUI();

	UPROPERTY()
	TSubclassOf<UWaveTimerWidget> WaveTimerWidgetClass;

private:
	UWaveTimerWidget* WaveTimerWidget;
};

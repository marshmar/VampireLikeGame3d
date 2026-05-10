// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"


/**
 * 
 */

class UWaveTimerWidget;

UCLASS()
class VAMPIRELIKE3D_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWaveTimerWidget> WaveTimerWidgetClass;

	virtual void BeginPlay() override;
private:
	UWaveTimerWidget* TimerWidget;
};

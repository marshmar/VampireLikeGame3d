// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WaveTimerWidget.generated.h"

/**
 * 
 */

class AWaveTimer;

UCLASS()
class VAMPIRELIKE3D_API UWaveTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemainingTimeText;

	UPROPERTY()
	AWaveTimer* WaveTimer;

	void UpdateTime(float RemainingTime);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetWaveTimer(AWaveTimer* InWaveTimer) { WaveTimer = InWaveTimer; }
};

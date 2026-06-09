#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class VAMPIRELIKE3D_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHpBar(float Current, float Max);
	void UpdateBossHpBar(float Current, float Max);
	void SetBossHpBar(bool bState);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpBar;  // 블루프린트 ProgressBar 이름이랑 일치해야 함

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BossHpBar;  // 블루프린트 ProgressBar 이름이랑 일치해야 함

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossName;
};

/*
* 웨이브 타이머
* 타이머 초기화, 틱에서 타이머 감소, 현재 타이머 받아오기, 일정 시간마다 페이즈 변경 이벤트 발생
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Systems/Wave/PhaseData.h"
#include "WaveTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, const FPhaseData&, PhaseData);

UCLASS()
class VAMPIRELIKE3D_API AWaveTimer : public AActor
{
	GENERATED_BODY()
	
public:	

	AWaveTimer();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UDataTable* PhaseDataTable;

	UPROPERTY(BlueprintAssignable)
	FOnPhaseChanged OnPhaseChanged;

protected:
	virtual void BeginPlay() override;

private:
	float RemainingTime;
	float WaveDuration;
	int32 CurrentPhaseIndex;
	void CheckPhase();

// Getter & Setter
public:
	float GetRemainingTime() const;
	void DecreaseRemainingTime(float Value);
};

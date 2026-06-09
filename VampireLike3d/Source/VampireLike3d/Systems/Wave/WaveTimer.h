#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Systems/Wave/PhaseData.h"
#include "WaveTimer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeEnded);

UCLASS()
class VAMPIRELIKE3D_API AWaveTimer : public AActor
{
	GENERATED_BODY()
	
public:	

	AWaveTimer();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(BlueprintAssignable)
	FOnTimeEnded OnTimeEnded;

	UPROPERTY(VisibleAnywhere)
	float RemainingTime;

	UPROPERTY(VisibleAnywhere)
	float WaveDuration;

	void EndTime() const;
// Getter & Setter
public:
	float GetRemainingTime() const;
	float GetElapsedTime() const;
	void DecreaseRemainingTime(float Value);

};

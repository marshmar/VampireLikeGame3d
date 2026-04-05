#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwapGaugeComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VAMPIRELIKE3D_API USwapGaugeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USwapGaugeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool HasEnoughGaugeForSwapAttack() const;
	void ConsumeGauge();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Gauge")
	float MaxGauge;

	UPROPERTY(VisibleAnywhere, Category = "Gauge")
	float CurGauge;

	UPROPERTY(VisibleAnywhere, Category = "Gauge")
	float SwapAttackCost;

	UPROPERTY(VisibleAnywhere, Category = "Gauge")
	float ChargeInterval;

	UPROPERTY(VisibleAnywhere, Category = "Gauge")
	float ChargeAmountPerTick;

	FTimerHandle ChargeTimerHandle;

	void StartChargeTimer();
	void OnChargeTimer();
	void ChargeGauge(float Amount);
};

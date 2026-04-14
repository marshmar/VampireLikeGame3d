#include "Systems/Party/SwapGaugeComponent.h"

USwapGaugeComponent::USwapGaugeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwapGaugeComponent::BeginPlay()
{
	Super::BeginPlay();	

	MaxGauge = 100.f;
	CurGauge = 90.f;
	SwapAttackCost = 30.f;
	ChargeInterval = 1.f;
	ChargeAmountPerTick = 1.f;

	StartChargeTimer();
}

void USwapGaugeComponent::StartChargeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		ChargeTimerHandle,
		this,
		&USwapGaugeComponent::OnChargeTimer,
		ChargeInterval,
		true
	);
}

void USwapGaugeComponent::OnChargeTimer() 
{
	ChargeGauge(ChargeAmountPerTick);
}

void USwapGaugeComponent::ChargeGauge(float Amount)
{
	CurGauge = FMath::Clamp(CurGauge + Amount, 0.f, MaxGauge);
	UE_LOG(LogTemp, Warning, TEXT("CurGage: %f"), CurGauge);
}

void USwapGaugeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USwapGaugeComponent::HasEnoughGaugeForSwapAttack() const
{
	return CurGauge >= SwapAttackCost;
}

void USwapGaugeComponent::ConsumeGauge()
{
	CurGauge -= SwapAttackCost;
}


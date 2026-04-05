#include "Systems/Party/SwapGaugeComponent.h"

/*
* 스왑 게이지 컴포넌트
* 캐릭터 교대 시 스왑 게이지 소모 x
* 스왑 게이지가 일정량 있을 경우 스왑게이지 소모하여 캐릭터 교대
* 스왑 게이지는 1초마다 1정도 차오름, 적을 처치할 시에 마찬가지로 차오름
*/
USwapGaugeComponent::USwapGaugeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwapGaugeComponent::BeginPlay()
{
	Super::BeginPlay();	

	MaxGauge = 100.f;
	CurGauge = 60.f;
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


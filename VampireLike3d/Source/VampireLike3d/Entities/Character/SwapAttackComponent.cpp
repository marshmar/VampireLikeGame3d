#include "Entities/Character/SwapAttackComponent.h"

USwapAttackComponent::USwapAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USwapAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USwapAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


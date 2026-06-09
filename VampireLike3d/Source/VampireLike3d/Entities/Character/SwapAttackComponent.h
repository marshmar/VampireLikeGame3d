#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwapAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAMPIRELIKE3D_API USwapAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USwapAttackComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;	
};

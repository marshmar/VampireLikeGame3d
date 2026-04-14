#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AutoTargetingInterface.generated.h"


UINTERFACE(MinimalAPI)
class UAutoTargetingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VAMPIRELIKE3D_API IAutoTargetingInterface
{
	GENERATED_BODY()

public:
	virtual void OnTargetingBegin() = 0;
	virtual void OnTargetingTick(float DeltaTime) = 0;
	virtual void OnTargetingEnd() = 0;
};

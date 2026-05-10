#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VAMPIRELIKE3D_API IPoolable
{
	GENERATED_BODY()

public:
	virtual void OnAcquired() = 0;
	virtual void OnReleased() = 0;
};

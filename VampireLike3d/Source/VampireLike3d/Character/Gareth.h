#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Gareth.generated.h"

UCLASS()
class VAMPIRELIKE3D_API AGareth : public ABaseCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void Attack() override;
};

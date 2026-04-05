#pragma once

#include "CoreMinimal.h"
#include "Entities/Character/BaseCharacter.h"
#include "Gareth.generated.h"

UCLASS()
class VAMPIRELIKE3D_API AGareth : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	virtual void SwapAttack() override;
	virtual void OnSwapAttackEffect(const FName& EffectName) override;
	virtual void OnSwapAttackHit() override;

protected:
	virtual void BasicAttack() override;
};

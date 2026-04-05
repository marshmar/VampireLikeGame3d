#pragma once

#include "CoreMinimal.h"
#include "Entities/Character/BaseCharacter.h"
#include "Revenant.generated.h"


UCLASS()
class VAMPIRELIKE3D_API ARevenant : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	virtual void BasicAttack() override;

	virtual void SwapAttack() override;
	virtual void OnSwapAttackEffect(const FName& EffectName) override;
	virtual void OnSwapAttackHit() override;
private:
	UPROPERTY(EditAnyWhere, Category = "Effects")
	UParticleSystem* LaserEffect;
};

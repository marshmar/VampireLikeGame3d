#pragma once

#include "CoreMinimal.h"
#include "Entities/Character/BaseCharacter.h"
#include "Interfaces/AutoTargetingInterface.h"
#include "Revenant.generated.h"


UCLASS()
class VAMPIRELIKE3D_API ARevenant : public ABaseCharacter, public IAutoTargetingInterface
{
	GENERATED_BODY()

public:
	ARevenant();

protected:
	virtual void BasicAttack() override;

	virtual void SwapAttack() override;
	virtual void OnSwapAttackEffect(const FName& EffectName) override;
	virtual void OnSwapAttackHit() override;

	virtual void OnTargetingBegin() override;
	virtual void OnTargetingTick(float DeltaTime) override;
	virtual void OnTargetingEnd() override;
private:
	UPROPERTY(EditAnyWhere, Category = "Effects")
	UParticleSystem* BasicAttackEffect;

	UPROPERTY()
	UParticleSystemComponent* ActiveTargetingParticle;

	void ApplyFanShapedAttack(const FVector& StartLocation, const FVector& ForwardDirection);
};

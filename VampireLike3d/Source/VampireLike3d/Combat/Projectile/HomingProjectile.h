#pragma once

#include "CoreMinimal.h"
#include "Combat/Projectile/Projectile.h"
#include "HomingProjectile.generated.h"

class USphereComponent;

UCLASS()
class VAMPIRELIKE3D_API AHomingProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AHomingProjectile();

	virtual void InitProjectileMoveComp() override;
	virtual void SetHomingTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ProjectileParticle;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UFUNCTION()
	void OnHit(const FHitResult& HitResult);
};

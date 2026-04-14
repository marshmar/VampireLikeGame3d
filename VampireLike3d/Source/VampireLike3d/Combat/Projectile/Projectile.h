#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class VAMPIRELIKE3D_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	float GetDamage() const;
	void SetDamage(float Value);
protected:
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void BeginPlay() override;
	virtual void InitProjectileMoveComp();
	
private:
	UPROPERTY(VisibleAnywhere)
	float Damage;
};

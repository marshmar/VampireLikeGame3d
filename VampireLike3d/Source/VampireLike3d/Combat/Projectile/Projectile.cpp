#include "Combat/Projectile/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AProjectile::GetDamage() const
{
	return this->Damage;
}

void AProjectile::SetDamage(float Value)
{
	this->Damage = Value;
}

void AProjectile::InitProjectileMoveComp(){ }


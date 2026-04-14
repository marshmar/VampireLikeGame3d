#include "Combat/Projectile/HomingProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CollisionDefinitions.h"

AHomingProjectile::AHomingProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Enemy, ECR_Block);
	//CollisionComponent->SetHiddenInGame(false);

	// Setup ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectMovementComponent"));
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AHomingProjectile::OnHit);
	InitProjectileMoveComp();

	// Setup ParticleComponent
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->SetTemplate(ProjectileParticle);
}

void AHomingProjectile::InitProjectileMoveComp()
{
	Super::InitProjectileMoveComp();

	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2500.f;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;	// ignore graivty
	ProjectileMovementComponent->HomingAccelerationMagnitude = 7000.f;
}

void AHomingProjectile::SetHomingTarget(AActor* Target)
{
	if (Target == nullptr)
	{
		return;
	}

	// Search for a subobject specifically named 'HomingTargetPoint'.
	// NOTE: The component must be named EXACTLY as 'HomingTargetPoint' in the Actor for this to succeed.
	const FName AimPointName("HomingTargetPoint");
    USceneComponent* AimPoint = Cast<USceneComponent>(Target->GetDefaultSubobjectByName(AimPointName));

	// Set the homing target to the found point, otherwise fallback to the Target's RootComponent.
    ProjectileMovementComponent->HomingTargetComponent = AimPoint ? AimPoint : Target->GetRootComponent();
}

void AHomingProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AHomingProjectile::OnHit(const FHitResult& HitResult)
{
	AActor* Target = HitResult.GetActor();
	if(!IsValid(Target))
	{
		Destroy();
		return;
	}

	IHitInterface* HitActor = Cast<IHitInterface>(Target);
	if (HitActor == nullptr)
	{
		Destroy();
		return;
	}

	HitActor->GetHit(HitResult.ImpactPoint);

	UGameplayStatics::ApplyDamage(
		Target,
		20.0f,
		GetInstigatorController(),
		this,
		UDamageType::StaticClass()
	);

	if (HitParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's HitParticle is not set"), *GetName())
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitParticle,
		GetActorLocation(),
		GetActorRotation()
	);

	Destroy();
}


#include "GravityField.h"
#include "Effects/PhasedEffectComponent.h"
#include "Components/SphereComponent.h"
#include "Entities/Enemies/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utils/CollisionDefinitions.h"

AGravityField::AGravityField()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(300.0f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Enemy, ECR_Overlap);
	//SphereComponent->SetHiddenInGame(false);
	
	// Setup PhasedEffectComponent
	EffectComponent = CreateDefaultSubobject<UPhasedEffectComponent>(TEXT("PhasedEffectComponent"));

}

void AGravityField::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind Delegates
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGravityField::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AGravityField::OnOverlapEnd);
	EffectComponent->OnEffectFinished.AddDynamic(this, &AGravityField::EndGravityField);

	EffectComponent->StartEffect();
}

void AGravityField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (!IsValid(Enemy))
	{
		return;
	}

	auto EnemyCharacterMovementComponent = Enemy->GetCharacterMovement();
	if (!IsValid(EnemyCharacterMovementComponent))
	{
		return;
	}

	// Apply slow effect
	OriginSpeed = EnemyCharacterMovementComponent->MaxWalkSpeed;
	EnemyCharacterMovementComponent->MaxWalkSpeed = SlowSpeed;
}

void AGravityField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor);
	if (!IsValid(Enemy))
	{
		return;
	}

	auto EnemyCharacterMovementComponent = Enemy->GetCharacterMovement();
	if (!IsValid(EnemyCharacterMovementComponent))
	{
		return;
	}

	// Revert original speed
	Enemy->GetCharacterMovement()->MaxWalkSpeed = OriginSpeed;
}

void AGravityField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AGravityField::GetIdleDuration() const
{
	return EffectComponent->GetIdleDuration();
}

void AGravityField::SetIdleDuration(float Value)
{
	EffectComponent->SetIdleDuration(Value);
}

void AGravityField::EndGravityField()
{
	Destroy();
}

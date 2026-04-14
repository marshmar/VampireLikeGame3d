#include "Entities/Character/Nullex/Nullex.h"
#include "GameFramework/SpringArmComponent.h"
#include "Entities/Character/CharacterAttributeComponent.h"
#include "Combat/Projectile/HomingProjectile.h"
#include "Combat/Summon/GravityField.h"
#include "Kismet/GameplayStatics.h"
ANullex::ANullex()
{
	AttributeComp->SetAttackRange(2000.f);
	AttributeComp->SetAttackSpeed(2.f);
	AttributeComp->SetAtk(20.f);
}

void ANullex::BasicAttack()
{
	const float AttackRange = AttributeComp->GetAttackRange();
	AActor* NearestEnemy = FindNearestEnemy(AttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	FTargetingTransform SpawnTransform;

	// NOTE: The socket name must be named EXACTLY as 'Muzzle_01' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle_01");
	SpawnTransform = GetTargetingTransform(NearestEnemy, FireSocket);
	RotateTo(SpawnTransform.Direction);

	if (SpawnBasicAttackProjectile(NearestEnemy, SpawnTransform.StartLocation, SpawnTransform.Rotation))
	{
		PlayMontage("BasicAttack", BasicAttackMontage);
		SpawnEffectAtLocation(BasicAttackEffect, SpawnTransform.StartLocation, SpawnTransform.Rotation);
	}
}

void ANullex::SwapAttack()
{
	const float SwapAttackRange = 3000.0f;
	AActor* NearestEnemy = FindNearestEnemy(SwapAttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	FVector ToEnemy = (NearestEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	RotateTo(ToEnemy);

	const FVector SpawnLocation = NearestEnemy->GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

	AGravityField* Field = GetWorld()->SpawnActorDeferred<AGravityField>(
		GravityField,
		FTransform(FRotator::ZeroRotator, SpawnLocation),
		this,
		this
	);

	Field->SetIdleDuration(5.0f);

	UGameplayStatics::FinishSpawningActor(Field, SpawnTransform);

	PlayMontage(FName("SwapAttack"), SwapAttackMontage);
}

bool ANullex::SpawnBasicAttackProjectile(AActor* Target, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (BasicAttackProjectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's BasicAttackProjectile is not set"), *GetName())
		return false;
	}

	AHomingProjectile* HomingProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(
		BasicAttackProjectile,
		FTransform(SpawnRotation, SpawnLocation),
		this,
		this
	);

	if (!IsValid(HomingProjectile))
	{
		UE_LOG(LogTemp, Warning, TEXT("HomingProjectile failed to spawn"))
		return false;
	}

	HomingProjectile->SetHomingTarget(Target);
	HomingProjectile->SetDamage(AttributeComp->GetAtk());
	UGameplayStatics::FinishSpawningActor(HomingProjectile, HomingProjectile->GetActorTransform());

	return true;
}

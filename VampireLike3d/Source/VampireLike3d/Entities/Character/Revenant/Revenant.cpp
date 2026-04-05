// Fill out your copyright notice in the Description page of Project Settings.

#include "Revenant.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

void ARevenant::BasicAttack()
{
	AActor* NearestEnemy = FindNearestEnemy(1000.f);
	if (!NearestEnemy) return;

	FVector StartLocation = GetMesh()->GetSocketLocation(FName("Muzzle"));
	FVector EndLocation = NearestEnemy->GetActorLocation();
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FRotator LaserRotation = Direction.Rotation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		ECC_Pawn,
		QueryParams
	);

	for (FHitResult& HitResult : HitResults)
	{
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			20.0f,
			this->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitActor = Cast<IHitInterface>(HitResult.GetActor());
		if (!HitActor) return;

		HitActor->GetHit(HitResult.ImpactPoint);
	}

	if (LaserEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			LaserEffect,
			StartLocation,   
			LaserRotation
		);
	}

	PlayMontage(FName("BasicAttack"), BasicAttackMontage);
}

void ARevenant::SwapAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Revenant swapAttack Called"));
	PlayMontage(FName("SwapAttack"), SwapAttackMontage);
}

void ARevenant::OnSwapAttackEffect(const FName& EffectName)
{
	if (SwapAttackEffects.IsEmpty())
	{
		return;
	}

	UParticleSystem** Effect = SwapAttackEffects.Find(EffectName);
	if (!Effect)
	{
		return;
	}

	AActor* NearestEnemy = FindNearestEnemy(3000.f);
	if (!NearestEnemy) return;

	FVector StartLocation = GetMesh()->GetSocketLocation(FName("Muzzle"));
	FVector EndLocation = NearestEnemy->GetActorLocation();
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FRotator LaserRotation = Direction.Rotation();

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		*Effect,
		StartLocation,
		LaserRotation
	);
}

void ARevenant::OnSwapAttackHit()
{
	AActor* NearestEnemy = FindNearestEnemy(1000.f);
	if (!NearestEnemy) return;

	FVector StartLocation = GetMesh()->GetSocketLocation(FName("Muzzle"));
	FVector EndLocation = NearestEnemy->GetActorLocation();
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	FRotator LaserRotation = Direction.Rotation();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		ECC_Pawn,
		QueryParams
	);

	for (FHitResult& HitResult : HitResults)
	{
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			20.0f,
			this->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitActor = Cast<IHitInterface>(HitResult.GetActor());
		if (!HitActor) return;

		HitActor->GetHit(HitResult.ImpactPoint);
	}
}

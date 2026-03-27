// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Revenant.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

void ARevenant::Attack()
{
	AActor* NearestEnemy = FindNearestEnemy();
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
}

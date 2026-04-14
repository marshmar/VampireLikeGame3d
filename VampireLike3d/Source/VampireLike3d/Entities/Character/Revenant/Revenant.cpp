// Fill out your copyright notice in the Description page of Project Settings.

#include "Revenant.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Entities/Character/BaseCharacterAnimInstance.h"
#include "Entities/Character/CharacterAttributeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

ARevenant::ARevenant()
{
	AttributeComp->SetAttackRange(1000.0f);
	AttributeComp->SetAttackSpeed(3.0f);
}


void ARevenant::BasicAttack()
{
	const float AttackRange = AttributeComp->GetAttackRange();

	AActor* NearestEnemy = FindNearestEnemy(AttackRange);
	if (!NearestEnemy)
	{
		return;
	}
	
	// NOTE: The socket name must be named EXACTLY as 'Muzzle' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle");
	FTargetingTransform TargetingTransform = GetTargetingTransform(NearestEnemy, FireSocket);;
	RotateTo(TargetingTransform.Direction);

	const FVector ForwardDirection = GetActorForwardVector();
	ApplyFanShapedAttack(TargetingTransform.StartLocation, ForwardDirection);
	SpawnEffectAtLocation(BasicAttackEffect, TargetingTransform.StartLocation, ForwardDirection.Rotation());
	PlayMontage(FName("BasicAttack"), BasicAttackMontage);

	/*
	// 부채꼴 경계선 (좌/우)
	FVector LeftBound = ForwardDirection.RotateAngleAxis(-AttackAngle, FVector::UpVector);
	FVector RightBound = ForwardDirection.RotateAngleAxis(AttackAngle, FVector::UpVector);

	DrawDebugLine(GetWorld(), StartLocation, StartLocation + LeftBound * AttackRange, FColor::Green, false, 2.f, 0, 2.f);
	DrawDebugLine(GetWorld(), StartLocation, StartLocation + RightBound * AttackRange, FColor::Green, false, 2.f, 0, 2.f);

	// 부채꼴 호 (여러 선분으로 표현)
	int32 ArcSegments = 12;
	for (int32 i = 0; i < ArcSegments; i++)
	{
		float Alpha1 = (float)i / ArcSegments;
		float Alpha2 = (float)(i + 1) / ArcSegments;
		float Angle1 = FMath::Lerp(-AttackAngle, AttackAngle, Alpha1);
		float Angle2 = FMath::Lerp(-AttackAngle, AttackAngle, Alpha2);

		FVector ArcPoint1 = StartLocation + ForwardDirection.RotateAngleAxis(Angle1, FVector::UpVector) * AttackRange;
		FVector ArcPoint2 = StartLocation + ForwardDirection.RotateAngleAxis(Angle2, FVector::UpVector) * AttackRange;

		DrawDebugLine(GetWorld(), ArcPoint1, ArcPoint2, FColor::Green, false, 2.f, 0, 2.f);
	}

	// SweepMulti 탐지 범위
	DrawDebugCylinder(GetWorld(), StartLocation, StartLocation + ForwardDirection * AttackRange, 300.f, 16, FColor::Red, false, 2.f);

	// 피격 액터
	for (AActor* Actor : HitActors)
	{
		DrawDebugSphere(GetWorld(), Actor->GetActorLocation(), 30.f, 8, FColor::Yellow, false, 2.f);
	}

	*/
}

void ARevenant::ApplyFanShapedAttack(const FVector& StartLocation, const FVector& ForwardDirection)
{
	const float AttackRange = AttributeComp->GetAttackRange();
	const float AttackAngle = 30.f;
	const float Damage = 20.f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		StartLocation + ForwardDirection * (AttackRange * 0.5f), // 부채꼴 중심점
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRange),
		QueryParams
	);

	// Tracks actors already hit to prevent applying damage more than once
	TSet<AActor*> HitActors;

	for (FOverlapResult& Overlap : OverlapResults)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!IsValid(HitActor) || HitActors.Contains(HitActor))
		{
			continue;
		}

		// Filter by fan angle - skip actors outside the attack arc
		FVector ToTarget = (HitActor->GetActorLocation() - StartLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(ForwardDirection, ToTarget);
		float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
		if (AngleToTarget > AttackAngle) continue;

		HitActors.Add(HitActor);

		UGameplayStatics::ApplyDamage(
			HitActor,
			Damage,
			GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitInterface = Cast<IHitInterface>(HitActor);
		if (HitInterface == nullptr)
		{
			continue;
		}
		HitInterface->GetHit(HitActor->GetActorLocation());
	}
}

void ARevenant::SwapAttack()
{
	PlayMontage(FName("SwapAttack"), SwapAttackMontage);
}

void ARevenant::OnSwapAttackEffect(const FName& EffectName)
{
	if (SwapAttackEffects.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's SwapAttackEffects is not set"), *GetName())
		return;
	}

	UParticleSystem** Effect = SwapAttackEffects.Find(EffectName);
	if (Effect == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cannot find SwapAttackEffect"), *GetName())
		return;
	}

	const float SwapAttackRange = 3000.0f;
	AActor* NearestEnemy = FindNearestEnemy(SwapAttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	// NOTE: The socket name must be named EXACTLY as 'Muzzle' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle");
	FTargetingTransform TargetTransform = GetTargetingTransform(NearestEnemy, FireSocket);

	SpawnEffectAtLocation(*Effect, TargetTransform.StartLocation, TargetTransform.Rotation);
}

void ARevenant::OnSwapAttackHit()
{
	const float SwapAttackRange = 3000.f;
	const float Damage = 20.f;
	AActor* NearestEnemy = FindNearestEnemy(SwapAttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	

	// NOTE: The socket name must be named EXACTLY as 'Muzzle' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle");
	FTargetingTransform TargetingTransform = GetTargetingTransform(NearestEnemy, FireSocket);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByChannel(
		HitResults,
		TargetingTransform.StartLocation,
		TargetingTransform.EndLocation,
		ECC_Pawn,
		QueryParams
	);

	for (FHitResult& HitResult : HitResults)
	{
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			Damage,
			this->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitActor = Cast<IHitInterface>(HitResult.GetActor());
		if (HitActor == nullptr)
		{
			return;
		}

		HitActor->GetHit(HitResult.ImpactPoint);
	}
}


void ARevenant::OnTargetingBegin()
{
	const float SwapAttackRange = 3000.f;
	AActor* NearestEnemy = FindNearestEnemy(SwapAttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	// NOTE: The socket name must be named EXACTLY as 'Muzzle' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle");
	FTargetingTransform TargetingTransform = GetTargetingTransform(NearestEnemy, FireSocket);

	// 레이저 이펙트 소환
	UParticleSystem** Effect = SwapAttackEffects.Find("SwapAttackStart");
	if (Effect == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s cannot find SwapAttackEffect"), *GetName())
		return;
	}

	ActiveTargetingParticle = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		*Effect,
		TargetingTransform.StartLocation,
		TargetingTransform.Rotation,
		true
	);

	// 적 방향으로 회전
	SetActorRotation(FRotator(0.f, TargetingTransform.Rotation.Yaw, 0.f));
}

void ARevenant::OnTargetingTick(float DeltaTime)
{
	const float SwapAttackRange = 3000.f;
	AActor* NearestEnemy = FindNearestEnemy(SwapAttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	if (ActiveTargetingParticle == nullptr || !ActiveTargetingParticle->IsActive())
	{
		return;
	}

	// NOTE: The socket name must be named EXACTLY as 'Muzzle' in the SkeletalMesh for this to succeed.
	const FName FireSocket("Muzzle");
	FTargetingTransform TargetingTransform = GetTargetingTransform(NearestEnemy, FireSocket);

	ActiveTargetingParticle->SetWorldLocationAndRotation(TargetingTransform.StartLocation, TargetingTransform.Rotation);

	FRotator TargetRotation = FRotator(0.f, TargetingTransform.Rotation.Yaw, 0.f);
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.f);
	SetActorRotation(NewRotation);
}

void ARevenant::OnTargetingEnd()
{
}



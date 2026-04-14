#include "Gareth.h"
#include "Kismet/GameplayStatics.h"
#include "Entities/Character/CharacterAttributeComponent.h"
#include "Interfaces/HitInterface.h"
AGareth::AGareth()
{
	AttributeComp->SetAttackSpeed(1.0f);
	AttributeComp->SetAttackRange(1000.0f);
}

void AGareth::BasicAttack()
{
	const float AttackRange = 300.0f;
	AActor* NearestEnemy = FindNearestEnemy(AttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}

	PlayMontage(FName("BasicAttack"), BasicAttackMontage);
}

void AGareth::SwapAttack()
{
	PlayMontage(FName("SwapAttack"), SwapAttackMontage);
}

void AGareth::OnSwapAttackEffect(const FName& EffectName)
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

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		*Effect,
		GetActorLocation()
	);
}

void AGareth::OnSwapAttackHit()
{
	TArray<FHitResult> HitResults;
	FVector Center = GetActorLocation();
	const float HitRange = 1000.0f; 
	const float Damge = 60.0f;

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(), Center, Center, HitRange,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false, TArray<AActor*>(),
		EDrawDebugTrace::None, // EDrawDebugTrace::ForDuration
		HitResults, true);


	for (FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor))
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(
			HitActor,
			Damge,
			GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitInterface = Cast<IHitInterface>(HitActor);
		if (HitInterface == nullptr)
		{
			continue;
		}
		HitInterface->GetHit(HitResult.ImpactPoint);
	}
}

void AGareth::OnSwapAttackMove()
{
	const float AttackRange = 3000.0f;
	AActor* NearestEnemy = FindNearestEnemy(AttackRange);
	if (!IsValid(NearestEnemy))
	{
		return;
	}
	SetActorLocation(NearestEnemy->GetActorLocation());
}

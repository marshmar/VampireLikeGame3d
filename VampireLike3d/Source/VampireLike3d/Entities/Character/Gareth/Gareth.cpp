#include "Gareth.h"
#include "Kismet/GameplayStatics.h"
#include "Entities/Character/CharacterAttributeComponent.h"
#include "Interfaces/HitInterface.h"
#include "Utils/CollisionDefinitions.h"

AGareth::AGareth()
{
	CharAttributeComp->SetAttackSpeed(1.0f);
	CharAttributeComp->SetAttackRange(1000.0f);
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

	// 이펙트 범위만큼 오버랩 체크
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(HitRange); // 이펙트 크기랑 맞추기

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_Enemy, // Player만 감지
		CollisionShape
	);

	//DrawDebugSphere(
	//	GetWorld(),
	//		Center,
	//		HitRange,
	//	12,
	//	FColor::Red,
	//	false,  // 지속 표시 여부
	//	2.f     // 표시 시간 (초)
	//);


	TArray<AActor*> HitActors; // 중복 방지

	for (FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!IsValid(HitActor)) continue;

		// 중복 피격 방지
		if (HitActors.Contains(HitActor)) continue;
		HitActors.Add(HitActor);

		IHitInterface* HitInterface = Cast<IHitInterface>(HitActor);
		if (!HitInterface) continue;

		HitInterface->GetHit(AttributeComp->GetAtk(), Center);
	}

	//UKismetSystemLibrary::SphereTraceMulti(
	//	GetWorld(), Center, Center, HitRange,
	//	UEngineTypes::ConvertToTraceType(ECC_Enemy),
	//	false, TArray<AActor*>(),
	//	EDrawDebugTrace::None, // EDrawDebugTrace::ForDuration
	//	HitResults, true);


	//for (FHitResult& HitResult : HitResults)
	//{
	//	AActor* HitActor = HitResult.GetActor();
	//	if (!IsValid(HitActor))
	//	{
	//		continue;
	//	}

	//	IHitInterface* HitInterface = Cast<IHitInterface>(HitActor);
	//	if (HitInterface == nullptr)
	//	{
	//		continue;
	//	}
	//	HitInterface->GetHit(Damge, HitResult.ImpactPoint);
	//}
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

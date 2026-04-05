#include "Gareth.h"
#include "Kismet/GameplayStatics.h"

void AGareth::BasicAttack()
{
	AActor* NearestEnemy = FindNearestEnemy(1000.f);
	if (!NearestEnemy) return;

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
		return;
	}

	UParticleSystem** Effect = SwapAttackEffects.Find(EffectName);
	if (!Effect)
	{
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
	float Radius = 1000.0f; // 이 값을 기준으로 이펙트 크기 맞추기

	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(), Center, Center, Radius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false, TArray<AActor*>(),
		EDrawDebugTrace::ForDuration, // 디버그로 범위 시각화
		HitResults, true);
}

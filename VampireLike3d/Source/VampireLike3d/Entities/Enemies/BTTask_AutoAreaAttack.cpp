#include "Entities/Enemies/BTTask_AutoAreaAttack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BossEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

UBTTask_AutoAreaAttack::UBTTask_AutoAreaAttack()
{
	NodeName = TEXT("AutoAreaAttack");
}


EBTNodeResult::Type UBTTask_AutoAreaAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossEnemy* Boss = Cast<ABossEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert boss"));
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!IsValid(Target))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get TargetActor"));
		return EBTNodeResult::Failed;
	}

	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = Boss->GetActorLocation().Z + 0.1f;

	// Spawn red range decal
	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		WarningDecalMaterial,
		FVector(WarningRadius, WarningRadius, WarningRadius),
		TargetLocation,
		FRotator(-90.f, 0.f, 0.f),
		WarningDuration
	);

	// Dynamic Material Instance 생성
	UMaterialInstanceDynamic* DynMat =
		UMaterialInstanceDynamic::Create(WarningDecalMaterial, this);
	Decal->SetMaterial(0, DynMat);

	// 타이머로 FillAmount 0 → 1로 증가
	FillAmount = 0.5f;

	Boss->GetWorldTimerManager().SetTimer(
		FillTimerHandle,
		[this, Boss, DynMat, TargetLocation]()
		{
			FillAmount = FMath::Max(0.01f, FillAmount - 0.03f);

			DynMat->SetScalarParameterValue(TEXT("FillAmount"), FillAmount);

			if (FillAmount <= 0.2f && !bIsEffectSpawned)
			{
				// 이펙트 소환
				UParticleSystemComponent* Effect = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					AttackEffect,
					TargetLocation,
					FRotator::ZeroRotator,
					FVector(3.f)
				);

				FTimerDelegate TimerDel;
				TimerDel.BindLambda([Boss, TargetLocation]()
					{
						Boss->DamageToPlayer(TargetLocation, 200.f);
					});

				Boss->GetWorldTimerManager().SetTimer(HitTimerHandle, TimerDel, 0.6f, false);
				bIsEffectSpawned = true;
			}
			// 다 채워지면 이펙트 소환
			if (FillAmount <= 0.01f)
			{
			    // 타이머 종료
				Boss->GetWorldTimerManager().ClearTimer(FillTimerHandle);
				FillAmount = 0.5; // 다음 공격을 위해 초기화
				bIsEffectSpawned = false;
			}
		},
		0.05f, // 0.05초마다 업데이트
		true   // 반복
	);

	return EBTNodeResult::Succeeded;
}
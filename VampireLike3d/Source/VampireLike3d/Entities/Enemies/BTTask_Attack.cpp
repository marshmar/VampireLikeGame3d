#include "Entities/Enemies/BTTask_Attack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BossEnemy.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Boss Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// choose random attack
	int32 AttackIndex = FMath::RandRange(0, 2);

	UAnimMontage* MontageToPlay = nullptr;
	MontageToPlay = Boss->SmashMontage;

	if (MontageToPlay == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Montage didn't set up"));
		return EBTNodeResult::Failed;
	}
	
	// turn to target
	//OwnerComp.GetAIOwner()->SetFocus(Target);
	FVector Direction = Target->GetActorLocation()- Boss->GetActorLocation();
	Direction.Z = 0.f; 
	FRotator LookAt = Direction.Rotation();
	Boss->SetActorRotation(LookAt);

	// play montage
	Boss->PlayAnimMontage(MontageToPlay);

	SpawnShockwave(Boss, Target);

	// register montage callback
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([&OwnerComp, this](UAnimMontage* Montage, bool bInterrupted)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
	Boss->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, MontageToPlay);

	UE_LOG(LogTemp, Warning, TEXT("Attack In Progress"));
	// wait until montage end
	return EBTNodeResult::InProgress;

	//TODO : 
	// 1. 플레이어가 보스 공격 범위에 있는지 판정
	// 2. 보스 공격 쿨타임 설정
	// 3. 보스 공격 시에 충돌 판정,
}

void UBTTask_Attack::SpawnShockwave(ABossEnemy* BossClass, AActor* Target)
{
	FVector StartLocation = BossClass->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();
	const float Distance = FVector::Dist(StartLocation, TargetLocation) + 3000.0f;
	const float EffectInterval = 100.f;
	const int32 EffectCnt = FMath::CeilToInt(Distance / EffectInterval);

	for (int32 i = 0; i < EffectCnt; i++)
	{
		FVector SpawnLocation = StartLocation + Direction * (i * EffectInterval);
		float Delay = i * 0.025f;
		FTimerHandle TempTimer;
		BossClass->GetWorldTimerManager().SetTimer(
			TempTimer,
			[BossClass, SpawnLocation]()
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					BossClass->GetWorld(),
					BossClass->ShockwaveEffect,
					SpawnLocation
				);

				FTimerHandle TempHitHandle;
				FTimerDelegate TimerDel;
				TimerDel.BindLambda([BossClass, SpawnLocation]()
					{
						BossClass->DamageToPlayer(SpawnLocation, 100.f);
					});
				BossClass->GetWorldTimerManager().SetTimer(TempHitHandle, TimerDel, 0.2f, false);
			},
			Delay,
			false
		);
	}
	
	
	// 4. 보스와 플레이어의 일직선 방향으로 이펙트를 순차적으로 생성(인터벌 필요)
}

//void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
//{
//}
//
//void UBTTask_Attack::OnMontageEnded(UBehaviorTreeComponent* OwnerComp)
//{
//}

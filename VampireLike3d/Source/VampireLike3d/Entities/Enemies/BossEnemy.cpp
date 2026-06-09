#include "Entities/Enemies/BossEnemy.h"
#include "BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Utils/CollisionDefinitions.h"
#include "Entities/AttributeComponent.h"
#include "Systems/UI/UIManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AutoAreaAttack.h"

ABossEnemy::ABossEnemy()
{
	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
}

void ABossEnemy::Die()
{
	if (OnBossDied.IsBound())
	{
		OnBossDied.Broadcast();
	}
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	AttributeComp->SetMaxHP(1000.f);
	AttributeComp->SetCurHP(AttributeComp->GetMaxHP());

	// 스폰될 때 움직임 막기
	GetCharacterMovement()->DisableMovement();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnSpawnMontageEnded);
		AnimInstance->StopAllMontages(0.f);

		// 스폰 몽타주 재생
		if (SpawnMontage)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnSpawnMontageEnded);
			FName Section = FName("Spawn");
			PlayMontage(Section, SpawnMontage);

			// 몽타주 끝나면 BT 시작
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
				{
					ABossAIController* AIC = Cast<ABossAIController>(GetController());
					if (AIC)
					{
						AIC->StartBehaviorTree(); // BT 시작
					}

				});

			GetMesh()->GetAnimInstance()
				->Montage_SetEndDelegate(EndDelegate, SpawnMontage);
		}
	}
}

void ABossEnemy::EnterPhase2()
{
	bIsPhase2 = true;

	// 몽타주 재생
	if (!EnterPhase2Montage) return;

	// BT 잠깐 멈추기
	ABossAIController* AIC = Cast<ABossAIController>(GetController());
	if (AIC) AIC->GetBrainComponent()->StopLogic(TEXT("EnterPhase2"));

	PlayAnimMontage(EnterPhase2Montage);

	// 몽타주 끝나면 페이즈 2 진입
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this, AIC](UAnimMontage* Montage, bool bInterrupted)
		{
			// 이동속도 증가
			GetCharacterMovement()->MaxWalkSpeed *= Phase2SpeedMultiplier;

			//// 공격 주기 빠르게
			//AttackInterval *= Phase2AttackIntervalMultiplier;

			// 블랙보드 업데이트
			if (AIC)
			{
				AIC->GetBlackboardComponent()
					->SetValueAsBool(BBKeys::bIsPhase2, true);

				// BT 재개
				AIC->GetBrainComponent()->RestartLogic();
			}

			UE_LOG(LogTemp, Warning, TEXT("Phase 2 Start!"));
		});

	GetMesh()->GetAnimInstance()
		->Montage_SetEndDelegate(EndDelegate, EnterPhase2Montage);
}

void ABossEnemy::DamageToPlayer(FVector TargetLocation, float Range)
{
	// 이펙트 범위만큼 오버랩 체크
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Range); // 이펙트 크기랑 맞추기

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		TargetLocation,
		FQuat::Identity,
		ECC_Player, // Player만 감지
		CollisionShape
	);

	// 디버그 스피어
	//DrawDebugSphere(
	//	GetWorld(),
	//	TargetLocation,
	//	Range,
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

		HitInterface->GetHit(AttributeComp->GetAtk(), TargetLocation);
	}

}

void ABossEnemy::GetHit(float DamageAmount, const FVector& ImpactPoint)
{
	Super::GetHit(DamageAmount, ImpactPoint);

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->UpdateBossHpBar(AttributeComp->GetCurHP(), AttributeComp->GetMaxHP());
	}

	if (!bIsPhase2 && AttributeComp->GetCurHP() / AttributeComp->GetMaxHP() <= Phase2HpThreshold)
	{
		EnterPhase2();
	}
}

UBehaviorTree* ABossEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}

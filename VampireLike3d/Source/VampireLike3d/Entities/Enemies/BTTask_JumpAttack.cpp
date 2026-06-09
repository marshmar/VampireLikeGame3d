#include "Entities/Enemies/BTTask_JumpAttack.h"
#include "Entities/Enemies/BossEnemy.h" 
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"

UBTTask_JumpAttack::UBTTask_JumpAttack()
{
    NodeName = TEXT("Jump Attack");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_JumpAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: AIController is invalid"));
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIController->GetPawn();
	if (!IsValid(ControlledPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: ControlledPawn is invalid"));
		return EBTNodeResult::Failed;
	}

	ABossEnemy* Boss = Cast<ABossEnemy>(ControlledPawn);
	if (!IsValid(Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: Boss cast failed"));
		return EBTNodeResult::Failed;
	}

	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get TargetActor"));
		return EBTNodeResult::Failed;
	}

	FJumpAttackTaskMemory* Memory = reinterpret_cast<FJumpAttackTaskMemory*>(NodeMemory);
	if (!Memory)
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: Memory is invalid"));
		return EBTNodeResult::Failed;
	}

	FVector StartLocation = Boss->GetActorLocation();
	FVector TargetLocation = Player->GetActorLocation();
	TargetLocation.Z = StartLocation.Z;

	Memory->Boss = Boss;
	Memory->StartLocation = StartLocation;
	Memory->TargetLocation = TargetLocation;
	Memory->ElapsedTime = 0.f;
	Memory->bHasDamaged = false;
	Memory->bPlayedLoopMontage = false;
	Memory->bPlayedSlamMontage = false;
	Memory->State = EJumpAttackState::Roar;

	UCharacterMovementComponent* Movement = Boss->GetCharacterMovement();
	if (IsValid(Movement))
	{
		Movement->StopMovementImmediately();
		Movement->DisableMovement();
	}

	UAnimInstance* AnimInstance = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: AnimInstance is invalid"));

		if (IsValid(Movement))
		{
			Movement->SetMovementMode(MOVE_Walking);
		}

		return EBTNodeResult::Failed;
	}

	UE_LOG(LogTemp, Warning, TEXT("JumpAttack Roar Start"));
	UE_LOG(LogTemp, Warning, TEXT("Start Location: %s"), *StartLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *TargetLocation.ToString());

	if (RoarMontage)
	{
		AIController->SetFocus(Player);

		UBehaviorTreeComponent* OwnerCompPtr = &OwnerComp;

		FOnMontageEnded RoarEndDelegate;
		RoarEndDelegate.BindLambda(
			[this, OwnerCompPtr, Memory, Boss](UAnimMontage* Montage, bool bInterrupted)
			{
				UE_LOG(LogTemp, Warning, TEXT("JumpAttack Roar Delegate Called"));

				if (!IsValid(OwnerCompPtr))
				{
					return;
				}

				if (!IsValid(Boss))
				{
					FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
					return;
				}

				if (bInterrupted)
				{
					UE_LOG(LogTemp, Warning, TEXT("JumpAttack Roar Montage Interrupted"));

					UCharacterMovementComponent* Movement = Boss->GetCharacterMovement();
					if (IsValid(Movement))
					{
						Movement->SetMovementMode(MOVE_Walking);
					}

					FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
					return;
				}


				APawn* CurrentPlayer = Cast<APawn>(OwnerCompPtr->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
				if (!IsValid(CurrentPlayer))
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to get TargetActor"));
					return;
				}

				/*APawn* CurrentPlayer = UGameplayStatics::GetPlayerPawn(Boss->GetWorld(), 0);
				if (!IsValid(CurrentPlayer))
				{
					UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: CurrentPlayer is invalid after roar"));
					FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
					return;
				}*/

				FVector NewStartLocation = Boss->GetActorLocation();
				FVector NewTargetLocation = CurrentPlayer->GetActorLocation();
				NewTargetLocation.Z = NewStartLocation.Z;

				Memory->StartLocation = NewStartLocation;
				Memory->TargetLocation = NewTargetLocation;
				Memory->ElapsedTime = 0.f;
				Memory->bHasDamaged = false;
				Memory->bPlayedLoopMontage = false;
				Memory->bPlayedSlamMontage = false;
				Memory->State = EJumpAttackState::Jump;

				if (JumpStartMontage)
				{
					Boss->PlayAnimMontage(JumpStartMontage);
				}

				UE_LOG(LogTemp, Warning, TEXT("JumpAttack Roar Ended"));
				UE_LOG(LogTemp, Warning, TEXT("Jump Start Location: %s"), *Memory->StartLocation.ToString());
				UE_LOG(LogTemp, Warning, TEXT("Jump Target Location: %s"), *Memory->TargetLocation.ToString());
			}
		);

		// 1. 먼저 몽타주 재생
		float MontageLength = Boss->PlayAnimMontage(RoarMontage);

		UE_LOG(LogTemp, Warning, TEXT("Roar Montage Play Length: %f"), MontageLength);

		if (MontageLength <= 0.f)
		{
			UE_LOG(LogTemp, Error, TEXT("JumpAttack Failed: RoarMontage did not play"));

			if (IsValid(Movement))
			{
				Movement->SetMovementMode(MOVE_Walking);
			}

			return EBTNodeResult::Failed;
		}

		// 2. 재생된 몽타주에 EndDelegate 등록
		AnimInstance->Montage_SetEndDelegate(RoarEndDelegate, RoarMontage);
	}
	else
	{
		FVector NewStartLocation = Boss->GetActorLocation();
		FVector NewTargetLocation = Player->GetActorLocation();
		NewTargetLocation.Z = NewStartLocation.Z;

		Memory->StartLocation = NewStartLocation;
		Memory->TargetLocation = NewTargetLocation;
		Memory->State = EJumpAttackState::Jump;
		Memory->ElapsedTime = 0.f;

		if (JumpStartMontage)
		{
			Boss->PlayAnimMontage(JumpStartMontage);
		}

		UE_LOG(LogTemp, Warning, TEXT("JumpAttack No RoarMontage. Jump Start Immediately"));
		UE_LOG(LogTemp, Warning, TEXT("Jump Start Location: %s"), *Memory->StartLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Jump Target Location: %s"), *Memory->TargetLocation.ToString());
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_JumpAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FJumpAttackTaskMemory* Memory = reinterpret_cast<FJumpAttackTaskMemory*>(NodeMemory);
	if (!Memory)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ABossEnemy* Boss = Memory->Boss;
	if (!IsValid(Boss))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 중요: 포효 중에는 점프 이동 금지
	if (Memory->State == EJumpAttackState::Roar)
	{
		return;
	}

	Memory->ElapsedTime += DeltaSeconds;

	float Alpha = Memory->ElapsedTime / TotalJumpTime;
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

	// 공중 루프 몽타주
	if (!Memory->bPlayedLoopMontage && Alpha >= LoopMontageStartAlpha)
	{
		if (JumpLoopMontage)
		{
			Boss->PlayAnimMontage(JumpLoopMontage);
		}

		Memory->bPlayedLoopMontage = true;
		UE_LOG(LogTemp, Warning, TEXT("JumpAttack Loop Montage Played"));
	}

	// 내려찍기 몽타주
	if (!Memory->bPlayedSlamMontage && Alpha >= SlamMontageStartAlpha)
	{
		if (JumpSlamMontage)
		{
			Boss->PlayAnimMontage(JumpSlamMontage);
		}

		Memory->bPlayedSlamMontage = true;
		UE_LOG(LogTemp, Warning, TEXT("JumpAttack Slam Montage Played"));
	}

	FVector NewLocation = FMath::Lerp(
		Memory->StartLocation,
		Memory->TargetLocation,
		Alpha
	);

	float HeightOffset = FMath::Sin(Alpha * PI) * JumpHeight;
	NewLocation.Z += HeightOffset;

	Boss->SetActorLocation(NewLocation);

	if (Alpha >= 1.f)
	{
		Boss->SetActorLocation(Memory->TargetLocation);

		// 착지 이펙트 소환
		if (LandingEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				Boss->GetWorld(),
				LandingEffect,
				Memory->TargetLocation,
				FRotator::ZeroRotator,
				LandingEffectScale
			);

			UE_LOG(LogTemp, Warning, TEXT("JumpAttack Landing Effect Spawned"));
		}

		if (!Memory->bHasDamaged)
		{
			Boss->DamageToPlayer(Memory->TargetLocation, DamageRadius);
			Memory->bHasDamaged = true;
		}

		UCharacterMovementComponent* Movement = Boss->GetCharacterMovement();
		if (IsValid(Movement))
		{
			Movement->SetMovementMode(MOVE_Walking);
		}

		UE_LOG(LogTemp, Warning, TEXT("JumpAttack Success"));
		UE_LOG(LogTemp, Warning, TEXT("Boss Landed Location: %s"), *Boss->GetActorLocation().ToString());

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_JumpAttack::GetInstanceMemorySize() const
{
	return sizeof(FJumpAttackTaskMemory);
}

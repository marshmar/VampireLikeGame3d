#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "Entities/AttributeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/Party/PartyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CollisionDefinitions.h"
#include "Particles/ParticleSystem.h"
//#include "Kismet/KismetSystemLibrary.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Player, ECR_Ignore); // ← 추가

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Player, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
	GetCharacterMovement()->bPushForceScaledToMass = false;     // 밀기 힘 끔

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Set MoveSpeed
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetMoveSpeed();

	// Setup HomingTargetComponent
	// NOTE: The component name must be named EXACTLY as 'HomingTargetPoint' to success for homing
	HomingTargetPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HomingTargetPoint"));
	HomingTargetPoint->SetupAttachment(RootComponent);
	HomingTargetPoint->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeComp->SetMaxHP(20.f);
	AttributeComp->SetCurHP(AttributeComp->GetMaxHP());
	AttributeComp->SetAtk(20.f);

	APartyManager* PartyManager = Cast<APartyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APartyManager::StaticClass()));
	if (IsValid(PartyManager))
	{
		PartyManager->OnPlayerSwapped.AddUObject(this, &ABaseEnemy::UpdateTarget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find PartyManager class in world"));
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnPlayerOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnPlayerOverlapEnd);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{	

	UE_LOG(LogTemp, Warning, TEXT("Object overlap detectioned"))
	OverlappingPlayer = Cast<ABaseCharacter>(OtherActor);
	if (OverlappingPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is in"));
		// 타이머가 이미 실행 중이면 중복 등록 방지
		if (GetWorldTimerManager().IsTimerActive(DamageTimerHandle)) return;

		UE_LOG(LogTemp, Warning, TEXT("Set timer"));
		ApplyDamageToPlayer();
		//GetWorldTimerManager().SetTimer(
		//	DamageTimerHandle,
		//	this,
		//	&ABaseEnemy::ApplyDamageToPlayer,
		//	1.0f,  // 1초마다
		//	true   // 반복
		//);
	}
}

void ABaseEnemy::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ABaseCharacter>(OtherComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is out"));
		OverlappingPlayer = nullptr;
		//GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}

void ABaseEnemy::GetHit(float DamageAmount, const FVector& ImpactPoint)
{
	GetCharacterMovement()->DisableMovement();

	Super::GetHit(DamageAmount, ImpactPoint);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnHitReactEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnHitReactEnded);
	}
}

void ABaseEnemy::UpdateTarget(APawn* NewCharacter)
{
	if (NewCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to update target becase of NewCharacter is nullptr"));
		return;
	}

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("%s's AIController didn't set up"), *GetName());
		return;
	}

	AIController->UpdateTarget(NewCharacter);
}

void ABaseEnemy::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != DeathMontage) return;

	HandleDeath();
}

void ABaseEnemy::ApplyDamageToPlayer()
{
	if (OverlappingPlayer)
	{
		OverlappingPlayer->GetHit(AttributeComp->GetAtk(), GetActorLocation());
	}
}

void ABaseEnemy::OnHitReactEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ABaseEnemy::OnAcquired()
{
	// HP 초기화
	AttributeComp->SetCurHP(AttributeComp->GetMaxHP());
	DeathPose = EDeathPose::EDP_Alive;
	//UE_LOG(LogTemp, Warning, TEXT("MaxHP: %f, CurHP: %f"),
	//	AttributeComp->GetMaxHP(), AttributeComp->GetCurHP());

	// Set MoveSpeed
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetMoveSpeed();

	// 스폰될 때 움직임 막기
	GetCharacterMovement()->DisableMovement();

	// 충돌 활성화
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnSpawnMontageEnded);
		AnimInstance->StopAllMontages(0.f);

		// 스폰 몽타주 재생
		if (SpawnMontage)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnSpawnMontageEnded);
			FName Section = FName("Spawn");
			PlayMontage(Section, SpawnMontage);
		}
	}


}

void ABaseEnemy::OnReleased()
{
	OnEnemyDied.Clear();

	//UE_LOG(LogTemp, Warning, TEXT("%s's OnReleased was called"), *GetName());

	// 몽타주 콜백 해제
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s's OnReleased was succesfully called"), *GetName());
}


void ABaseEnemy::Die()
{
	Super::Die();

	// 사망 시 완전히 멈춤
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// 충돌 비활성화
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	// AI 비활성화
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
	}

}

void ABaseEnemy::OnSpawnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != SpawnMontage) return;

	// 스폰 몽타주 끝나면 움직임 재개
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnSpawnMontageEnded);
	}
}

void ABaseEnemy::HandleDeath()
{
	// 이펙트 소환
	if (DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
	}

	if (OnEnemyDied.IsBound())
	{
		OnEnemyDied.Broadcast(this->GetClass());
	}

	UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
	if (PoolManager)
	{
		PoolManager->ReleaseActorByClass(this);
	}
}



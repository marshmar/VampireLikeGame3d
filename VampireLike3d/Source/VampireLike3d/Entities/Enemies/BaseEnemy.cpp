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
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Setup AttributeComponent
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComp->SetMoveSpeed(75.0f);

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
	
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay 호출"));
	AttributeComp->SetMaxHP(20.f);
	AttributeComp->SetCurHP(AttributeComp->GetMaxHP());

	APartyManager* PartyManager = Cast<APartyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APartyManager::StaticClass()));
	if (IsValid(PartyManager))
	{
		PartyManager->OnPlayerSwapped.AddUObject(this, &ABaseEnemy::UpdateTarget);
	}
}

void ABaseEnemy::PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || AnimMontage == nullptr)
	{
		return;
	}
	AnimInstance->Montage_Play(AnimMontage);
	AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
}


void ABaseEnemy::Die(const FVector& ImpactPoint)
{
	// 사망 시 완전히 멈춤
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// AI 비활성화
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}

	const float Theta = CalculateHitDegree(ImpactPoint);
	FName Section("DeathBack");
	DeathPose = EDeathPose::EDP_DeathBack;
	if (Theta >= -90.f && Theta < 90.f)
	{
		Section = FName("DeathForward");
		DeathPose = EDeathPose::EDP_DeathForward;
	}
	PlayMontage(Section, DeathMontage);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 중복 방지
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
	}
}



void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseEnemy::GetHit(const FVector& ImpactPoint)
{
	if (AttributeComp->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die(ImpactPoint);
	}
}

float ABaseEnemy::CalculateHitDegree(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward| |TOHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void ABaseEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	// 피격 시 잠깐 움직임 멈춤
	GetCharacterMovement()->DisableMovement();

	// 히트 리액션 몽타주 끝나면 움직임 재개
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		float MontageDuration = AnimInstance->Montage_Play(HitReactMontage);
		GetWorldTimerManager().SetTimer(
			HitReactTimerHandle,
			this,
			&ABaseEnemy::OnHitReactEnded,
			MontageDuration,
			false
		);
	}

	const float Theta = CalculateHitDegree(ImpactPoint);

	// Hit Direction Guide
	// https://github.com/user-attachments/assets/e92be45f-3fdb-4a39-a2f5-f1246c1d1425
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	PlayMontage(Section, HitReactMontage);

	#pragma region Debug
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 300, 5.f, FColor::Blue, 5.f);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	//}

	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 100, 5.f, FColor::Green, 5.f);
	#pragma endregion
}

void ABaseEnemy::UpdateTarget(APawn* NewCharacter)
{
	if (NewCharacter == nullptr)
	{
		return;
	}

	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (!IsValid(AIController))
	{
		return;
	}

	AIController->UpdateTarget(NewCharacter);
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// TODO: AttributeComp에서 데미지 깎기 전에 버프/디버프 같은 효과 적용 필요
	AttributeComp->ReceiveDamage(DamageAmount);
	return 0.0f;
}

void ABaseEnemy::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != DeathMontage) return;

	// 이펙트 소환
	if (DeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation());
	}

	if (OnEnemyDied.IsBound())
	{
		OnEnemyDied.Broadcast();
	}

	UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
	if (PoolManager)
	{
		PoolManager->ReleaseActorByClass(this);
	}


}

void ABaseEnemy::OnHitReactEnded()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ABaseEnemy::OnAcquired()
{
	// HP 초기화
	AttributeComp->SetCurHP(AttributeComp->GetMaxHP());
	DeathPose = EDeathPose::EDP_Alive;
	UE_LOG(LogTemp, Warning, TEXT("MaxHP: %f, CurHP: %f"),
		AttributeComp->GetMaxHP(), AttributeComp->GetCurHP());

	// 스폰될 때 움직임 막기
	GetCharacterMovement()->DisableMovement();

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

	// 몽타주 콜백 해제
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABaseEnemy::OnDeathMontageEnded);
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



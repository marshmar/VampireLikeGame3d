#include "BaseEnemy.h"
#include "EnemyAIController.h"
#include "Entities/AttributeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/Party/PartyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/CollisionDefinitions.h"
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
	
	AttributeComp->SetMaxHP(100.f);
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
	// TODO: Play Death Montage
	const float Theta = CalculateHitDegree(ImpactPoint);
	FName Section("DeathBack");
	if (Theta >= -90.f && Theta < 90.f)
	{
		Section = FName("DeathForward");
		DeathPose = EDeathPose::EDP_DeathForward;
	}
	PlayMontage(Section, DeathMontage);
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




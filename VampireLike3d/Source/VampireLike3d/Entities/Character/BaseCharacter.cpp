#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/Party/PartyManager.h"
#include "Entities/Enemies/BaseEnemy.h"
#include "CharacterAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacterAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Setup SpringArmComponent
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 300.f));
	CameraBoom->bUsePawnControlRotation = true;

	// Setup CameraComponent
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false;
	ViewCamera->SetRelativeLocation(FVector(-200.f, 0.f, 0.f));
	ViewCamera->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	// Setup Attribute Component
	AttributeComp = CreateDefaultSubobject<UCharacterAttributeComponent>(TEXT("CharacterAttributeComponent"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	PartyManager = Cast<APartyManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APartyManager::StaticClass()));
	if (!IsValid(PartyManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's PartyManager failed to assign"), *GetName())
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAction(FName("Swap"), IE_Pressed, this, &ABaseCharacter::SwapCharacter);
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Controller == nullptr || Value == 0.f)
	{
		return;
	}

	// find out which way is forward
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Controller == nullptr || Value == 0.f)
	{
		return;
	}

	// find out which way is right
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::SwapCharacter()
{
	if (!IsValid(PartyManager))
	{
		return;
	}

	PartyManager->SwapCharacterToNext();
	StopAttackTimer();
}

void ABaseCharacter::BasicAttack(){ }
void ABaseCharacter::SwapAttack(){ }
void ABaseCharacter::OnSwapAttackEffect(const FName& EffectName){ }
void ABaseCharacter::OnSwapAttackHit(){ }
void ABaseCharacter::OnSwapAttackMove(){ }

bool ABaseCharacter::IsSwapAttacking() const
{
	return SwapAttacking == true;
}

void ABaseCharacter::SetSwapAttacking(bool State)
{
	SwapAttacking = State;
}

/// Rotates the character to the specified direction while keeping the camera in place.
void ABaseCharacter::RotateTo(FVector& Direction)
{
	FRotator CameraWorldRotation = CameraBoom->GetComponentRotation();
	SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
	CameraBoom->SetWorldRotation(CameraWorldRotation);
}

void ABaseCharacter::PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr || AnimMontage == nullptr)
	{
		return;
	}

	AnimInstance->Montage_Play(AnimMontage);
	AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
}

AActor* ABaseCharacter::FindNearestEnemy(float Distance)
{
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Distance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const FVector CharacterLocation = GetActorLocation();
	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		CharacterLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape,
		QueryParams
	);

	AActor* NearestEnemy = nullptr;
	float MinDistance = FLT_MAX;

	for (FOverlapResult& Overlap : Overlaps)
	{
		AActor* Enemy = Cast<ABaseEnemy>(Overlap.GetActor());
		if (!IsValid(Enemy))
		{
			continue;
		}

		float Distance = FVector::Dist(CharacterLocation, Enemy->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestEnemy = Enemy;
		}
	}

	return NearestEnemy;
}

void ABaseCharacter::SpawnEffectAtLocation(UParticleSystem* Effect, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (Effect == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's %s is not set"), *GetName(), *Effect->GetName())
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		Effect,
		SpawnLocation,
		SpawnRotation
	);
}

FTargetingTransform ABaseCharacter::GetTargetingTransform(AActor* Target, const FName& SocketName) const
{
	FTargetingTransform OutData;

	// Get the Socket location from the Mesh
	OutData.StartLocation = GetMesh()->GetSocketLocation(SocketName);

	// Get Target Location
	OutData.EndLocation = Target->GetActorLocation();

	// Calculate Direction and Rotaion
	OutData.Direction = (OutData.EndLocation - OutData.StartLocation).GetSafeNormal();
	OutData.Rotation = OutData.Direction.Rotation();

	return OutData;
}

void ABaseCharacter::StartAttackTimer()
{
	GetWorldTimerManager().SetTimer(
		BasicAttackTimerHandle,
		this,
		&ABaseCharacter::BasicAttack,
		AttributeComp->GetAttackSpeed(),
		true
	);
}

void ABaseCharacter::StopAttackTimer()
{
	GetWorldTimerManager().ClearTimer(BasicAttackTimerHandle);
}

void ABaseCharacter::SetCameraBoomPawnControlRotation(bool State)
{
	if (CameraBoom == nullptr)
	{
		return;
	}
	CameraBoom->bUsePawnControlRotation = State;
}




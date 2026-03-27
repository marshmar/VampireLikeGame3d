// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PartyManager.h"
#include "Enemies/BaseEnemy.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 100.f));
	CameraBoom->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PartyManager = GetGameInstance()->GetSubsystem<UPartyManager>();
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
	if (Controller && (Value != 0.f))
	{
		// find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Controller && (Value != 0.f))
	{
		// find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::SwapCharacter()
{
	if (PartyManager)
	{
		PartyManager->SwapCharacterToNext();
		UE_LOG(LogTemp, Warning, TEXT("Swap to next Character"));
	}

	StopAttackTimer();
}

void ABaseCharacter::Attack()
{
	AActor* NearestEnemy = FindNearestEnemy();
	if (!NearestEnemy)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Object Name: %s Attack to %s"), *GetName(), *(NearestEnemy->GetName()));
}

AActor* ABaseCharacter::FindNearestEnemy()
{
	const float AttackDist = 1000.0f;
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackDist);	// 각 캐릭터별 공격 사거리로 변경 필요.

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const FVector CharLocation = GetActorLocation();
	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		CharLocation,
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
		if (!Enemy)
		{
			continue;
		}

		float Distance = FVector::Dist(CharLocation, Enemy->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestEnemy = Enemy;
		}
	}

	return NearestEnemy;
}

void ABaseCharacter::StartAttackTimer()
{
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABaseCharacter::Attack,
		3.0f,
		true
	);
}

void ABaseCharacter::StopAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

void ABaseCharacter::SetCameraBoomPawnControlRotation(bool State)
{
	if (!CameraBoom)
	{
		return;
	}
	CameraBoom->bUsePawnControlRotation = State;
}




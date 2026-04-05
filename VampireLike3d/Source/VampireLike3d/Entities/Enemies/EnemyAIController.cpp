// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// UPartyManager inherits from GameInstance, and since SpawnCharacter() is called
	// in PartyManager's BeginPlay(), PlayerPawn is guaranteed to exist at this point.
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	const float AcceptanceRadius = 3.0f;
	Super::Tick(DeltaTime);
	if (PlayerPawn)
	{
		MoveToActor(PlayerPawn, AcceptanceRadius);
	}
}

void AEnemyAIController::UpdateTarget(APawn* NewCharacter)
{
	// No need to null-check NewCharacter here, as it is already validated in Enemy's UpdateTarget.
	PlayerPawn = NewCharacter;
}



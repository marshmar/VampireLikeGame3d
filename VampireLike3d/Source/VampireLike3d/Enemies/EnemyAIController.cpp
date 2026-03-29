// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn Ã£À½!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn NULL!"));
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerPawn)
	{
		MoveToActor(PlayerPawn, 3.0f);
	}
}



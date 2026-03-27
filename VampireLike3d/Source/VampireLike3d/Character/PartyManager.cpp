// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PartyManager.h"
#include "BaseCharacter.h"

void UPartyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ActiveIndex = 0;
}

void UPartyManager::Deinitialize()
{
	SpawnedPartyMembers.Empty();
	PartyMembers.Empty();

	Super::Deinitialize();
}

void UPartyManager::SpawnPartyMembers()
{
	for (int32 i = 0; i < PartyMembers.Num(); i++)
	{
		if (!PartyMembers[i])
		{
			continue;
		}

		ABaseCharacter* Spawned = GetWorld()->SpawnActor<ABaseCharacter>(
			PartyMembers[i], FVector::ZeroVector, FRotator::ZeroRotator);

		if (Spawned)
		{
			if (i != 0)
			{
				Spawned->SetActorHiddenInGame(true);
				Spawned->SetActorEnableCollision(false);
				Spawned->SetActorTickEnabled(false);
			}
			SpawnedPartyMembers.Add(Spawned);
		}
	}
}

void UPartyManager::AddPartyMember(TSubclassOf<ABaseCharacter> Character)
{
	if (!Character)
	{
		return;
	}

	if (PartyMembers.Num() >= 3)
	{
		return;
	}

	PartyMembers.Add(Character);
}

void UPartyManager::SwapCharacter(int32 SlotIndex)
{
	if (!SpawnedPartyMembers.IsValidIndex(SlotIndex))
	{
		return;
	}

	ABaseCharacter* NewCharacter = SpawnedPartyMembers[SlotIndex];
	if (!NewCharacter)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	FRotator CameraRotation = FRotator::ZeroRotator;

	if (ActiveIndex != SlotIndex)
	{
		ABaseCharacter* CurrentCharacter = SpawnedPartyMembers[ActiveIndex];
		if (!CurrentCharacter)
		{
			return;
		}

		CameraRotation = PlayerController->GetControlRotation();

		NewCharacter->SetActorLocation(CurrentCharacter->GetActorLocation());
		NewCharacter->SetActorRotation(CurrentCharacter->GetActorRotation());
			
		// Disable Current Character
		CurrentCharacter->SetActorHiddenInGame(true);
		CurrentCharacter->SetActorEnableCollision(false);
		CurrentCharacter->SetActorTickEnabled(false);

		// Enable New Character
		NewCharacter->SetActorHiddenInGame(false);
		NewCharacter->SetActorEnableCollision(true);
		NewCharacter->SetActorTickEnabled(true);

	}
	
	NewCharacter->SetCameraBoomPawnControlRotation(false);
	PlayerController->Possess(NewCharacter);
	PlayerController->SetControlRotation(CameraRotation);
	NewCharacter->SetCameraBoomPawnControlRotation(true);
	NewCharacter->StartAttackTimer();
	ActiveIndex = SlotIndex;
}

void UPartyManager::SwapCharacterToNext()
{
	SwapCharacter((ActiveIndex + 1) % SpawnedPartyMembers.Num());	
}

void UPartyManager::SwapCharacterToBef()
{
	SwapCharacter(((ActiveIndex - 1) + SpawnedPartyMembers.Num()) % SpawnedPartyMembers.Num());
}

ABaseCharacter* UPartyManager::GetCurrentCharacter()
{
	return SpawnedPartyMembers[ActiveIndex];
}

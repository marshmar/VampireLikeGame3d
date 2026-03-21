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
	PartyMembers.Empty();
	Super::Deinitialize();
}

void UPartyManager::AddPartyMember(ABaseCharacter* Character)
{
	if (!Character) return;
	if (PartyMembers.Num() >= 3) return;


	PartyMembers.Add(Character);
}

void UPartyManager::SwapCharacter(int32 SlotIndex)
{
	if (!PartyMembers.IsValidIndex(SlotIndex)) return;

	ABaseCharacter* NewCharacter = PartyMembers[SlotIndex];
	if (!NewCharacter) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	UE_LOG(LogTemp, Warning, TEXT("ActiveIndex: %d, SlotIndex: %d, PartySize: %d"),
		ActiveIndex, SlotIndex, PartyMembers.Num());

	FRotator CameraRotation = FRotator::ZeroRotator;

	if (ActiveIndex != SlotIndex)
	{
		ABaseCharacter* CurrentCharacter = PartyMembers[ActiveIndex];
		if (!CurrentCharacter) return;

		UE_LOG(LogTemp, Warning, TEXT("비활성화: %s"), *CurrentCharacter->GetName());

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
	ActiveIndex = SlotIndex;
}

void UPartyManager::SwapCharacterToNext()
{
	SwapCharacter((ActiveIndex + 1) % PartyMembers.Num());	
}

void UPartyManager::SwapCharacterToBef()
{

}

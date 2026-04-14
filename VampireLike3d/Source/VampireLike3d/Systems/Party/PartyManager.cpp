#include "PartyManager.h"
#include "SwapGaugeComponent.h"

APartyManager::APartyManager()
{
	// Set up Components;
	SwapGaugeComp = CreateDefaultSubobject<USwapGaugeComponent>(TEXT("SwapGaugeComponent"));

	ActiveIndex = 0;
	MaxPartyMember = 3;
}

void APartyManager::BeginPlay()
{
	Super::BeginPlay();
}

void APartyManager::SpawnPartyMembers()
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

	PossesCharacter(0);
}

void APartyManager::AddPartyMember(TSubclassOf<ABaseCharacter> Character)
{
	if (!Character)
	{
		return;
	}

	if (PartyMembers.Num() >= MaxPartyMember)
	{
		return;
	}

	PartyMembers.Add(Character);
}

void APartyManager::SwapCharacter(int32 SlotIndex)
{
	HandlePreviousCharacter(SlotIndex);
	TransferTransform(SlotIndex);
	PossesCharacter(SlotIndex);
}

void APartyManager::SwapCharacterToNext()
{
	int32 StartIndex = ActiveIndex;
	int32 NextIndex = (ActiveIndex + 1) % SpawnedPartyMembers.Num();

	while (!IsValidSwap(NextIndex))
	{
		NextIndex = (NextIndex + 1) % SpawnedPartyMembers.Num();
		if (NextIndex == StartIndex)
		{
			return;
		}
	}
	SwapCharacter(NextIndex);
}

void APartyManager::SwapCharacterToPrev()
{
	int32 StartIndex = ActiveIndex;
	int32 PrevIndex = ((ActiveIndex - 1) + SpawnedPartyMembers.Num()) % SpawnedPartyMembers.Num();

	while (!IsValidSwap(PrevIndex))
	{
		PrevIndex = ((PrevIndex - 1) + SpawnedPartyMembers.Num()) % SpawnedPartyMembers.Num();
		if (PrevIndex == StartIndex)
		{
			return;
		}
	}
	SwapCharacter(PrevIndex);
}

ABaseCharacter* APartyManager::GetCurrentCharacter()
{
	return SpawnedPartyMembers[ActiveIndex];
}

void APartyManager::DisableCharacter(ABaseCharacter* Character)
{
	Character->SetActorHiddenInGame(true);
	Character->SetActorEnableCollision(false);
	Character->SetActorTickEnabled(false);
}

void APartyManager::EnableCharacter(ABaseCharacter* Character)
{
	Character->SetActorHiddenInGame(false);
	Character->SetActorEnableCollision(true);
	Character->SetActorTickEnabled(true);
}

bool APartyManager::IsValidSwap(int32 SlotIndex)
{
	if (!SpawnedPartyMembers.IsValidIndex(SlotIndex))
	{
		return false;
	}

	if (!SpawnedPartyMembers[SlotIndex])
	{
		return false;
	}

	if (ActiveIndex == SlotIndex)
	{
		return false;
	}

	if (SpawnedPartyMembers[SlotIndex]->IsSwapAttacking())
	{
		return false;
	}

	return true;
}

void APartyManager::HandlePreviousCharacter(int32 SlotIndex)
{
	ABaseCharacter* CurrentCharacter = SpawnedPartyMembers[ActiveIndex];
	if (!CurrentCharacter)
	{
		return;
	}

	if (!SwapGaugeComp)
	{
		return;
	}

	if (SwapGaugeComp->HasEnoughGaugeForSwapAttack())
	{
		CurrentCharacter->SetActorEnableCollision(false);
		CurrentCharacter->SwapAttack();

		CurrentCharacter->OnSwapAttackEnded.AddLambda([this, CurrentCharacter]()
			{
				DisableCharacter(CurrentCharacter);
				CurrentCharacter->OnSwapAttackEnded.Clear();
			});

		SwapGaugeComp->ConsumeGauge();
	}
	else
	{
		// Disable Current Character
		DisableCharacter(CurrentCharacter);
	}

}

void APartyManager::TransferTransform(int32 SlotIndex)
{
	ABaseCharacter* CurrentCharacter = SpawnedPartyMembers[ActiveIndex];
	if (!CurrentCharacter)
	{
		return;
	}

	ABaseCharacter* NewCharacter = SpawnedPartyMembers[SlotIndex]; // NewCharacter is guaranteed non-null here, as IsValidSwap already performs the null check.

	NewCharacter->SetActorLocation(CurrentCharacter->GetActorLocation());
	NewCharacter->SetActorRotation(CurrentCharacter->GetActorRotation());
}

void APartyManager::PossesCharacter(int32 SlotIndex)
{
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

	// Enable New Character
	EnableCharacter(NewCharacter);

	FRotator CameraRotation = FRotator::ZeroRotator;
	CameraRotation = PlayerController->GetControlRotation();

	NewCharacter->SetCameraBoomPawnControlRotation(false);
	PlayerController->Possess(NewCharacter);
	PlayerController->SetControlRotation(CameraRotation);
	NewCharacter->SetCameraBoomPawnControlRotation(true);
	NewCharacter->StartAttackTimer();
	ActiveIndex = SlotIndex;


	// Broadcast player swap event
	OnPlayerSwapped.Broadcast(Cast<APawn>(NewCharacter));
}

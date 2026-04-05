#include "PartyManager.h"
#include "SwapGaugeComponent.h"

APartyManager::APartyManager()
{
	// Set up Components;
	SwapGaugeComp = CreateDefaultSubobject<USwapGaugeComponent>(TEXT("SwapGaugeComponent"));
}

void APartyManager::BeginPlay()
{
	Super::BeginPlay();
	ActiveIndex = 0;
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
}

void APartyManager::AddPartyMember(TSubclassOf<ABaseCharacter> Character)
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

void APartyManager::SwapCharacter(int32 SlotIndex)
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


		CameraRotation = PlayerController->GetControlRotation();
		NewCharacter->SetActorLocation(CurrentCharacter->GetActorLocation());
		NewCharacter->SetActorRotation(CurrentCharacter->GetActorRotation());
		
		// Enable New Character
		EnableCharacter(NewCharacter);
	}


	NewCharacter->SetCameraBoomPawnControlRotation(false);
	PlayerController->Possess(NewCharacter);
	PlayerController->SetControlRotation(CameraRotation);
	NewCharacter->SetCameraBoomPawnControlRotation(true);
	NewCharacter->StartAttackTimer();
	ActiveIndex = SlotIndex;


	// Broadcast player swap event
	OnPlayerSwapped.Broadcast(Cast<APawn>(NewCharacter));
}

void APartyManager::SwapCharacterToNext()
{
	SwapCharacter((ActiveIndex + 1) % SpawnedPartyMembers.Num());	
}

void APartyManager::SwapCharacterToBef()
{
	SwapCharacter(((ActiveIndex - 1) + SpawnedPartyMembers.Num()) % SpawnedPartyMembers.Num());
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MyGameModeBase.h"
#include "Character/PartyManager.h"

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

    UPartyManager* PartyManager = GetGameInstance()->GetSubsystem<UPartyManager>();
    if (!PartyManager) return;

    TArray<TSubclassOf<ABaseCharacter>> Classes = {
        CharacterClass1, CharacterClass2, CharacterClass3
    };

    for (int32 i = 0; i < Classes.Num(); i++)
    {
        if (!Classes[i]) continue;

        ABaseCharacter* Spawned = GetWorld()->SpawnActor<ABaseCharacter>(
            Classes[i], FVector::ZeroVector, FRotator::ZeroRotator);

        if (Spawned)
        {
            if (i != 0)
            {
                Spawned->SetActorHiddenInGame(true);
                Spawned->SetActorEnableCollision(false);
                Spawned->SetActorTickEnabled(false);
            }
            PartyManager->AddPartyMember(Spawned);
        }
    }

    PartyManager->SwapCharacter(0);
}

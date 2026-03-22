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

		PartyManager->AddPartyMember(Classes[i]);
	}

	PartyManager->SpawnPartyMembers();
    PartyManager->SwapCharacter(0);
}

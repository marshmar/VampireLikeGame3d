#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Party/PartyManager.h"

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APartyManager* PartyManager = Cast<APartyManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APartyManager::StaticClass()));
	if (!PartyManager)
	{
		return;
	}

    TArray<TSubclassOf<ABaseCharacter>> Classes = {
        CharacterClass1, CharacterClass2, CharacterClass3
    };

	for (int32 i = 0; i < Classes.Num(); i++)
	{
		if (!Classes[i])
		{
			continue;
		}

		PartyManager->AddPartyMember(Classes[i]);
	}

	PartyManager->SpawnPartyMembers();
}

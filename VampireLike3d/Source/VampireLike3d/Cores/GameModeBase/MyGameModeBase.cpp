#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Party/PartyManager.h"
#include "Systems/UI/UIManager.h"
#include "Systems/ObjectPool/PoolManagerSubsystem.h"
#include "Entities/Enemies/BaseEnemy.h"
#include "Systems/Sound/SoundManagerSubsystem.h"

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APartyManager* PartyManager = Cast<APartyManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APartyManager::StaticClass()));
	if (IsValid(PartyManager))
	{
		// TODO: Current implementation uses hardcoded member variables for a small character set (3 types).
		// As the project scales, transition to a dedicated Data Manager class or Data Table 
		// to decouple character definitions from this class
		TArray<TSubclassOf<ABaseCharacter>> Classes = {
			CharacterClass1, CharacterClass2, CharacterClass3
		};

		for (int32 i = 0; i < Classes.Num(); i++)
		{
			if (Classes[i] == nullptr)
			{
				continue;
			}

			PartyManager->AddPartyMember(Classes[i]);
		}

		PartyManager->SpawnPartyMembers();
	}

	//UPoolManagerSubsystem* PoolManager = GetGameInstance()->GetSubsystem<UPoolManagerSubsystem>();
	//if (IsValid(PoolManager))
	//{
	//	PoolManager->InitializePool<ABaseEnemy>(50);
	//}

	//UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	//if (IsValid(UIManager))
	//{
	//}

	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if (IsValid(SoundManager))
	{
		//RegisterSounds(SoundManager);
	}
}

void AMyGameModeBase::RegisterSounds(USoundManagerSubsystem* SoundManager)
{
	SoundManager->RegisterSFX(ESFXType::BossRoar, BossRoarSound);
	SoundManager->RegisterSFX(ESFXType::BossJump, BossJumpSound);
	SoundManager->RegisterSFX(ESFXType::BossLanding, BossLandingSound);
}

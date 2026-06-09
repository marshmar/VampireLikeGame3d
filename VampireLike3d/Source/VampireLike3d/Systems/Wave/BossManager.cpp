#include "Systems/Wave/BossManager.h"
#include "Entities/Enemies/BossEnemy.h"
#include "WaveTimer.h"
#include "Systems/UI/UIManager.h"

ABossManager::ABossManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossManager::BeginPlay()
{
	Super::BeginPlay();
	
	BossSpawnTime = 5.f;
	bIsBossSpawned = false;


}

void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsBossSpawned && WaveTimer->GetElapsedTime() >= BossSpawnTime)
	{
		SpawnBoss();
		bIsBossSpawned = true;
	}
}

void ABossManager::SpawnBoss()
{
	if (!IsValid(BossClass))
	{
		UE_LOG(LogTemp, Error, TEXT("BossClass is nullptr"));
		return;
	}

	if (!IsValid(BossSpawnPoint))
	{
		UE_LOG(LogTemp, Error, TEXT("BossSpawnPoint was not assigned"));
		return;
	}

	FTransform SpawnTransform = BossSpawnPoint->GetActorTransform();
	CurrentBoss = GetWorld()->SpawnActor<ABossEnemy>(BossClass, SpawnTransform);

	if (IsValid(CurrentBoss))
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Successfully spawned"));
		CurrentBoss->OnBossDied.AddDynamic(this, &ABossManager::BroadcastBossDefeated); 

		if (OnBossSpawned.IsBound())
		{
			OnBossSpawned.Broadcast();
		}

		UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
		if (UIManager)
		{
			UIManager->SetBossHpBar(true);
		}

	}
}

void ABossManager::BroadcastBossDefeated()
{
	CurrentBoss = nullptr;

	if (OnBossDefeated.IsBound())
	{
		OnBossDefeated.Broadcast();
	}
}

void ABossManager::DestroyBoss()
{
	if (IsValid(CurrentBoss))
	{
		CurrentBoss->Destroy();
		CurrentBoss = nullptr;
	}
}


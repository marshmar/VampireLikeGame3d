#include "Systems/Sound/SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("SoundManagerSubsystem Initialized"));
}

void USoundManagerSubsystem::Deinitialize()
{
	SFXMap.Empty();

	UE_LOG(LogTemp, Warning, TEXT("SoundManagerSubsystem Deinitialized"));

	Super::Deinitialize();
}

void USoundManagerSubsystem::RegisterSFX(ESFXType SFXType, USoundBase* Sound)
{
    if (!Sound)
    {
        UE_LOG(LogTemp, Warning, TEXT("RegisterSFX failed: Sound is null"));
        return;
    }

    SFXMap.Add(SFXType, Sound);

    UE_LOG(LogTemp, Warning, TEXT("SFX Registered"));
}

void USoundManagerSubsystem::PlaySFX2D(ESFXType SFXType, float Volume)
{
    USoundBase* Sound = SFXMap.FindRef(SFXType);

    if (!Sound)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaySFX2D failed: Sound not found"));
        return;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaySFX2D failed: World is null"));
        return;
    }

    UGameplayStatics::PlaySound2D(World, Sound, Volume);
}

void USoundManagerSubsystem::PlaySFXAtLocation(ESFXType SFXType, FVector Location, float Volume)
{
    USoundBase* Sound = SFXMap.FindRef(SFXType);

    if (!Sound)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaySFXAtLocation failed: Sound not found"));
        return;
    }

    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlaySFXAtLocation failed: World is null"));
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(World, Sound, Location, Volume);
}

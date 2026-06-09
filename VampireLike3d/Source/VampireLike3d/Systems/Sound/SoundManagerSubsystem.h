#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "SoundManagerSubsystem.generated.h"

UENUM(BlueprintType)
enum class ESFXType : uint8
{
	ButtonClick     UMETA(DisplayName = "Button Click"),
	PlayerHit       UMETA(DisplayName = "Player Hit"),
	BossRoar        UMETA(DisplayName = "Boss Roar"),
	BossJump        UMETA(DisplayName = "Boss Jump"),
	BossLanding     UMETA(DisplayName = "Boss Landing"),
	Explosion       UMETA(DisplayName = "Explosion"),
	MainBGM			UMETA(DisplayName = "Main BGM"),
};


/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void RegisterSFX(ESFXType SFXType, USoundBase* Sound);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySFX2D(ESFXType SFXType, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySFXAtLocation(ESFXType SFXType, FVector Location, float Volume = 1.0f);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TMap<ESFXType, USoundBase*> SFXMap;
};

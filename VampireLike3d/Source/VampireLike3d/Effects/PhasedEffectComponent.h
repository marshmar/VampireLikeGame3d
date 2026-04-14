#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhasedEffectComponent.generated.h"

UENUM(BlueprintType)
enum class EEffectState : uint8
{
	None,
	Starting,
	Idle,
	Finishing,
	Done
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAMPIRELIKE3D_API UPhasedEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPhasedEffectComponent();

	/*
	* Deligates
	*/
	UPROPERTY()
	FOnEffectStarted OnEffectStarted;

	UPROPERTY()
	FOnEffectFinished OnEffectFinished;

	UFUNCTION()
	void StartEffect();

	UFUNCTION()
	void ForceStop();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetIdleDuration() const;
	void SetIdleDuration(float Value);
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* StartParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* IdleParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* FinishParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float IdleDuration;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ActiveParticleComponent;
	FTimerHandle IdleTimerHandle;
	EEffectState CurrentState = EEffectState::None;

	void PlayStartEffect();
	void PlayIdleEffect();
	void PlayFinishEffect();
	void CleanupActiveParticle();
	void SetState(EEffectState NewState);
	bool SpawnParticle(UParticleSystem* Particle);
	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* PSystem);
};

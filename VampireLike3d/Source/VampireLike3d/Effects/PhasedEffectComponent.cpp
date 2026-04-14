#include "Effects/PhasedEffectComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UPhasedEffectComponent::UPhasedEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPhasedEffectComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UPhasedEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ForceStop();
	Super::EndPlay(EndPlayReason);
}

void UPhasedEffectComponent::StartEffect()
{
	if (CurrentState != EEffectState::None && CurrentState != EEffectState::Done)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already playing"), *GetName());
		return;
	}

	PlayStartEffect();
}

void UPhasedEffectComponent::ForceStop()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
	CleanupActiveParticle();
	SetState(EEffectState::Done);
}

void UPhasedEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UPhasedEffectComponent::GetIdleDuration() const
{
	return this->IdleDuration;
}


void UPhasedEffectComponent::SetIdleDuration(float Value)
{
	this->IdleDuration = Value;
}

void UPhasedEffectComponent::SetState(EEffectState NewState)
{
	CurrentState = NewState;
}

/**
 * Spawns and attaches a particle system to the owner's root component.
 * @param Particle The particle system template to spawn.
 * @return True if the particle component was successfully created and is valid.
 */
bool UPhasedEffectComponent::SpawnParticle(UParticleSystem* Particle)
{
	ActiveParticleComponent = UGameplayStatics::SpawnEmitterAttached(
		Particle,
		GetOwner()->GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FVector(1.f),
		EAttachLocation::SnapToTarget,
		false
	);

	return IsValid(ActiveParticleComponent);
}


void UPhasedEffectComponent::PlayStartEffect()
{
	CleanupActiveParticle();

	if (StartParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's StartParticle is not set"), *GetName())
		return;
	}

	SetState(EEffectState::Starting);

	if (SpawnParticle(StartParticle))
	{
		ActiveParticleComponent->OnSystemFinished.AddDynamic(this, &UPhasedEffectComponent::OnParticleSystemFinished);
	}
}

void UPhasedEffectComponent::PlayIdleEffect()
{
	OnEffectStarted.Broadcast();
	CleanupActiveParticle();

	if (IdleParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's IdleParticle is not set"), *GetName())
		return;
	}

	SetState(EEffectState::Idle);

	if (SpawnParticle(IdleParticle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			IdleTimerHandle,
			this,
			&UPhasedEffectComponent::PlayFinishEffect,
			IdleDuration,
			false
		);
	}
}

void UPhasedEffectComponent::PlayFinishEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimerHandle);
	CleanupActiveParticle();

	if (FinishParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s's FinishParticle is not set"), *GetName())
		return;
	}

	SetState(EEffectState::Finishing);

	if (SpawnParticle(FinishParticle))
	{
		ActiveParticleComponent->OnSystemFinished.AddDynamic(this, &UPhasedEffectComponent::OnParticleSystemFinished);
	}
}

void UPhasedEffectComponent::OnParticleSystemFinished(UParticleSystemComponent* PSystem)
{
	switch (CurrentState)
	{
	case EEffectState::Starting:
		PlayIdleEffect();
		break;

	case EEffectState::Finishing:
		SetState(EEffectState::Done);
		ActiveParticleComponent = nullptr;
		OnEffectFinished.Broadcast();
		break;

	default:
		break;
	}
}

void UPhasedEffectComponent::CleanupActiveParticle()
{
	if (!IsValid(ActiveParticleComponent))
	{
		return;
	}

	ActiveParticleComponent->OnSystemFinished.RemoveDynamic(this, &UPhasedEffectComponent::OnParticleSystemFinished);
	ActiveParticleComponent->DeactivateSystem();
	ActiveParticleComponent->DestroyComponent();
	ActiveParticleComponent = nullptr;
}






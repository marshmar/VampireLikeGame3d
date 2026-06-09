#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Entities/Character/CharacterTypes.h"
#include "BaseEntity.generated.h"


class UAttributeComponent;
UCLASS()
class VAMPIRELIKE3D_API ABaseEntity : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	/*
	* FrameWork
	*/
	ABaseEntity();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* Hit
	*/
	virtual void GetHit(float DamageAmount, const FVector& ImpactPoint) override;
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	float CalculateHitDegree(const FVector& ImpactPoint);
	virtual void DirectionalDeath(const FVector& ImpactPoint);
	virtual void Die();
	virtual float CalculateFinalDamage(float DamageAmount);

	/*
	* Status
	*/
	virtual void InitializeStatus();
protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	/*
	* Animation Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	virtual void BeginPlay() override;
	void PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage);
	virtual void SpawnEffectAtLocation(UParticleSystem* Effect, const FVector& SpawnLocation, const FRotator& SpawnRotation);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComp;

private:

};

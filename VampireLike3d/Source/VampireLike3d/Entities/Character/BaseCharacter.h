#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Systems/ObjectPool/PoolManagerSubsystem.h"
#include "Entities/BaseEntity.h"
#include "BaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class APartyManager;
class UCharacterAttributeComponent;
class USwapAttackComponent;
class UBaseCharacterAnimInstance;

DECLARE_MULTICAST_DELEGATE(FOnSwapAttackEnded);

USTRUCT()
struct FTargetingTransform
{
	GENERATED_BODY()

	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Direction = FVector::ForwardVector;
};


UCLASS(Abstract)
class VAMPIRELIKE3D_API ABaseCharacter : public ABaseEntity
{
	GENERATED_BODY()

public:
	/*
	* Deligates
	*/
	FOnSwapAttackEnded OnSwapAttackEnded;

	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCameraBoomPawnControlRotation(bool State);
	virtual void StartAttackTimer();
	virtual void StopAttackTimer();

	virtual void SwapAttack();
	virtual void OnSwapAttackEffect(const FName& EffectName);
	virtual void OnSwapAttackHit();
	virtual void OnSwapAttackMove();

	bool IsSwapAttacking() const;
	void SetSwapAttacking(bool State);
	void RotateTo(FVector& Direction);

	/*
	* Hit
	*/
	virtual void GetHit(float DamageAmount, const FVector& ImpactPoint) override;
	virtual float CalculateFinalDamage(float DamageAmount) override;
	virtual void Die() override;
protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	APartyManager* PartyManager;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle BasicAttackTimerHandle;

	UPROPERTY(VisibleAnywhere)
	UCharacterAttributeComponent* SharedAttributeComp;

	UPROPERTY(VisibleAnywhere)
	UCharacterAttributeComponent* CharAttributeComp;
	/*
	* Anim Montage
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* BasicAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SwapAttackMontage;

	/*
	* Effects
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TMap<FName, UParticleSystem*> SwapAttackEffects;

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void SwapCharacter();
	virtual void BasicAttack();
	virtual AActor* FindNearestEnemy(float Distance);
	virtual FTargetingTransform GetTargetingTransform(AActor* Target, const FName& SocketName) const;

private:
	UPROPERTY(VisibleAnywhere)
	bool SwapAttacking;
};

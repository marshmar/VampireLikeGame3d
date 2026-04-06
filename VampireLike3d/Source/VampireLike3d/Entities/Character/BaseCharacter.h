#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class APartyManager;
class UCharacterAttributeComponent;
class UBaseCharacterAnimInstance;

DECLARE_MULTICAST_DELEGATE(FOnSwapAttackEnded);

UCLASS(Abstract)
class VAMPIRELIKE3D_API ABaseCharacter : public ACharacter
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
	UCharacterAttributeComponent* AttributeComp;

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

	/*
	* Play montage functions
	*/
	void PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage);
};

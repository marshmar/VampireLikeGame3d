// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UPartyManager;
class UCharacterAttributeComponent;

UCLASS(Abstract)
class VAMPIRELIKE3D_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCameraBoomPawnControlRotation(bool State);
	virtual void StartAttackTimer();
	virtual void StopAttackTimer();

protected:

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void SwapCharacter();
	virtual void Attack();
	virtual AActor* FindNearestEnemy();

	/*
	* Play montage functions
	*/
	void PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UPartyManager* PartyManager;

	UPROPERTY(VisibleAnywhere)
	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere)
	UCharacterAttributeComponent* AttributeComp;
	/*
	* Anim Montage
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UPartyManager;

USTRUCT()
struct FBaseStat
{
	GENERATED_BODY()

	// HP
	float MaxHP;
	float CurHP;
	float HPRecovery;
	float LifeSteal;
	float Shield;

	// Atk
	float AttackSpeed;
	float CriticalChance;
	float CriticalDamage;

	// Def
	float Armor;
	float DamageReduction;
	float DodgeChange;

	// Movement
	float MoveSpeed;

	// Utils
	float PickUpRadius;
	float Luck;
	float CoolDownDeduction; // Decrease Weapon Cool Time;

	// EXP
	float EXP;
	float EXPGainRate;
};

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

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPartyManager* PartyManager;
	FTimerHandle AttackTimerHandle;
};

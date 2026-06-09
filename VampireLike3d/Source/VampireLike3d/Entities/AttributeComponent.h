// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAMPIRELIKE3D_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	bool IsAlive() const;
	void ReceiveDamage(float Damage);
protected:
	virtual void BeginPlay() override;

private:

	/*
	* HP
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float MaxHP;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float CurHP;

	/*
	* ATK
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Atk;

	/*
	* DEF
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Armor;

	/*
	* Move
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float MoveSpeed;

#pragma region Getter & Setter
public:
	/*
	* HP
	*/
	float GetMaxHP() const;
	void SetMaxHP(float Value);
	float GetCurHP() const;
	void SetCurHP(float Value);

	/*
	* ATK
	*/
	float GetAtk() const;
	void SetAtk(float Value);

	/*
	* DEF
	*/
	float GetArmor() const;
	void SetArmor(float Value);

	/*
	* Move
	*/
	float GetMoveSpeed() const;
	void SetMoveSpeed(float Value);

#pragma endregion

};

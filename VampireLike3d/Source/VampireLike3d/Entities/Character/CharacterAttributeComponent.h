#pragma once

#include "CoreMinimal.h"
#include "Entities/AttributeComponent.h"
#include "CharacterAttributeComponent.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API UCharacterAttributeComponent : public UAttributeComponent
{
	GENERATED_BODY()
	
private:

	/*
	* HP
	*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float HPRecovery;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float LifeSteal;

	/*
	* DEF
	*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Shield;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float DamageReduction;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float DodgeChance;

	/*
	* ATK
	*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float AttackSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float CriticalChance;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float CriticalDamage;

	/*
	* Utils
	*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float PickUpRadius;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Luck;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float CoolDownDeduction;

	/*
	* EXP
	*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	int32 level;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float Exp;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float ExpGainRate;

#pragma region MyRegion

public:
	/*
	* HP
	*/
	float GetHpRecovery() const;
	void SetHpRecovery(float Value);
	float GetLifeSteal() const;
	void SetLifeSteal(float Value);

	/*
	* DEF
	*/
	float GetShield() const;
	void SetShield(float Value);
	float GetDamageReduction() const;
	void SetDamageReduction(float Value);
	float GetDodgeChance() const;
	void SetDodgeChance(float Value);

	/*
	* ATK
	*/
	float GetAttackSpeed() const;
	void SetAttackSpeed(float Value);
	float GetCriticalChance() const;
	void SetCriticalChance(float Value);
	float GetCriticalDamage() const;
	void SetCriticalDamage(float Value);

	/*
	* Utils
	*/
	float GetPickUpRadius() const;
	void SetPickUpRadius(float Value);
	float GetLuck() const;
	void SetLuck(float Value);
	float GetCoolDownDeduction() const;
	void SetCoolDownDeduction(float Value);

	/*
	* EXP
	*/
	int32 GetLevel() const;
	float GetExp() const;
	void SetExp(float Value);
	float GetExpGainRate() const;
	void SetExpGainRate(float Value);
#pragma endregion

};

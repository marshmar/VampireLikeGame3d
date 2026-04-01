#include "Character/CharacterAttributeComponent.h"

#pragma region Getter & Setter
/*
* HP
*/
float UCharacterAttributeComponent::GetHpRecovery() const
{
	return this->HPRecovery;
}
void UCharacterAttributeComponent::SetHpRecovery(float Value)
{
	this->HPRecovery = Value;
}
float UCharacterAttributeComponent::GetLifeSteal() const
{
	return this->LifeSteal;
}
void UCharacterAttributeComponent::SetLifeSteal(float Value)
{
	this->LifeSteal = Value;
}

/*
* DEF
*/
float UCharacterAttributeComponent::GetShield() const
{
	return this->Shield;
}

void UCharacterAttributeComponent::SetShield(float Value)
{
	this->Shield = Value;
}

float UCharacterAttributeComponent::GetDamageReduction() const
{
	return this->DamageReduction;
}

void UCharacterAttributeComponent::SetDamageReduction(float Value)
{
	this->DamageReduction = Value;
}

float UCharacterAttributeComponent::GetDodgeChance() const
{
	return this->DodgeChance;
}

void UCharacterAttributeComponent::SetDodgeChance(float Value)
{
	this->DodgeChance = Value;
}

/*
* ATK
*/
float UCharacterAttributeComponent::GetAttackSpeed() const
{
	return this->AttackSpeed;
}

void UCharacterAttributeComponent::SetAttackSpeed(float Value)
{
	this->AttackSpeed = Value;
}

float UCharacterAttributeComponent::GetCriticalChance() const
{
	return this->CriticalChance;
}

void UCharacterAttributeComponent::SetCriticalChance(float Value)
{
	this->CriticalChance = Value;
}

float UCharacterAttributeComponent::GetCriticalDamage() const
{
	return this->CriticalDamage;
}

void UCharacterAttributeComponent::SetCriticalDamage(float Value)
{
	this->CriticalDamage = Value;
}

/*
* Utils
*/
float UCharacterAttributeComponent::GetPickUpRadius() const
{
	return this->PickUpRadius;
}

void UCharacterAttributeComponent::SetPickUpRadius(float Value)
{
	this->PickUpRadius = Value;
}

float UCharacterAttributeComponent::GetLuck() const
{
	return this->Luck;
}

void UCharacterAttributeComponent::SetLuck(float Value)
{
	this->Luck = Value;
}

float UCharacterAttributeComponent::GetCoolDownDeduction() const
{
	return this->CoolDownDeduction;
}

void UCharacterAttributeComponent::SetCoolDownDeduction(float Value)
{
	this->CoolDownDeduction = Value;
}

/*
* EXP
*/
int32 UCharacterAttributeComponent::GetLevel() const
{
	return this->level;	
}
float UCharacterAttributeComponent::GetExp() const
{
	return this->Exp;
}
void UCharacterAttributeComponent::SetExp(float Value)
{
	this->Exp = Value;
}
float UCharacterAttributeComponent::GetExpGainRate() const
{
	return this->ExpGainRate;
}
void UCharacterAttributeComponent::SetExpGainRate(float Value)
{
	this->ExpGainRate = Value;
}
#pragma endregion



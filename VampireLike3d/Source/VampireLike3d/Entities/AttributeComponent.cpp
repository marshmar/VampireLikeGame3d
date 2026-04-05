
#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UAttributeComponent::IsAlive() const
{
	return this->CurHP > 0;
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurHP = FMath::Clamp(CurHP - Damage, 0.f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("%s health: %f"), *GetName(), CurHP);
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

/*
* Getter & Setter
*/
#pragma region Getter & Setter

/*
* HP
*/
float UAttributeComponent::GetMaxHP() const
{
	return this->MaxHP;
}

void UAttributeComponent::SetMaxHP(float Value)
{
	this->MaxHP = Value;
}

float UAttributeComponent::GetCurHP() const
{
	return this->CurHP;
}

void UAttributeComponent::SetCurHP(float Value)
{
	this->CurHP = Value;
}

/*
* ATK
*/
float UAttributeComponent::GetAtk() const
{
	return this->Atk;
}

void UAttributeComponent::SetAtk(float Value)
{
	this->Atk = Value;
}

/*
* DEF
*/
float UAttributeComponent::GetArmor() const
{
	return this->Armor;
}

void UAttributeComponent::SetArmor(float Value)
{
	this->Armor = Value;
}

/*
* Move
*/
float UAttributeComponent::GetMoveSpeed() const
{
	return this->MoveSpeed;
}

void UAttributeComponent::SetMoveSpeed(float Value)
{
	this->MoveSpeed = Value;
}

#pragma endregion


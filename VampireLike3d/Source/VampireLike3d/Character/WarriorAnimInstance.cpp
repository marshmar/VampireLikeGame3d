// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter)
	{
		WarriorCharacterMovement = BaseCharacter->GetCharacterMovement();
	}
}

void UWarriorAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (WarriorCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(WarriorCharacterMovement->Velocity);
	}
}

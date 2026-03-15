// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WarriorAnimInstance.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWarriorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		WarriorCharacterMovement = MainCharacter->GetCharacterMovement();
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

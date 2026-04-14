#include "Entities/Character/BaseCharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("BaseCharacter is not assigned"))
		return;
	}
	BaseCharacterMovement = BaseCharacter->GetCharacterMovement();
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacterMovement == nullptr)
	{
		return;
	}
	GroundSpeed = UKismetMathLibrary::VSizeXY(BaseCharacterMovement->Velocity);
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackStart()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	BaseCharacter->SetSwapAttacking(true);
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEnd()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	BaseCharacter->SetSwapAttacking(false);
	BaseCharacter->OnSwapAttackEnded.Broadcast();
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectStart()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	// NOTE: The effect name must be named EXACTLY as 'SwapAttackStart' to spawn effect.
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackStart"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectHit()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	// NOTE: The effect name must be named EXACTLY as 'SwapAttackHit' to spawn effect.
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackHit"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectEnd()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	// NOTE: The effect name must be named EXACTLY as 'SwapAttackEnd' to spawn effect
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackEnd"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackHit()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	BaseCharacter->OnSwapAttackHit();
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackMove()
{
	if (BaseCharacter == nullptr)
	{
		return;
	}
	BaseCharacter->OnSwapAttackMove();
}



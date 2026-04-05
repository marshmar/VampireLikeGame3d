#include "Entities/Character/BaseCharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter)
	{
		BaseCharacterMovement = BaseCharacter->GetCharacterMovement();
	}
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!BaseCharacterMovement)
	{
		return;
	}
	GroundSpeed = UKismetMathLibrary::VSizeXY(BaseCharacterMovement->Velocity);
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEnd()
{
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->OnSwapAttackEnded.Broadcast();
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectStart()
{
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackStart"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectHit()
{
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackHit"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackEffectEnd()
{
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->OnSwapAttackEffect(FName("SwapAttackEnd"));
}

void UBaseCharacterAnimInstance::AnimNotify_SwapAttackHit()
{
	if (!BaseCharacter)
	{
		return;
	}
	BaseCharacter->OnSwapAttackHit();
}



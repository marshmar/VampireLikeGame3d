#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

UCLASS()
class VAMPIRELIKE3D_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ABaseCharacter* BaseCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* BaseCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	float AimPitch;
	float AimYaw;

	/*
	* Animation Notifty
	*/
	UFUNCTION()
	virtual void AnimNotify_SwapAttackStart();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackEnd();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackEffectStart();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackEffectHit();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackEffectEnd();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackHit();

	UFUNCTION()
	virtual void AnimNotify_SwapAttackMove();
};

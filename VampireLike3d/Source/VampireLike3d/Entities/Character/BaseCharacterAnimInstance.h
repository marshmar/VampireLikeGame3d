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
	void AnimNotify_SwapAttackEnd();

	UFUNCTION()
	void AnimNotify_SwapAttackEffectStart();

	UFUNCTION()
	void AnimNotify_SwapAttackEffectHit();

	UFUNCTION()
	void AnimNotify_SwapAttackEffectEnd();

	UFUNCTION()
	void AnimNotify_SwapAttackHit();
};

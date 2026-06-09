#pragma once

#include "CoreMinimal.h"
#include "Entities/Enemies/BaseEnemy.h"
#include "BTTask_JumpAttack.h"
#include "BossEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDied);

class ABossAIController;
class UBehaviorTree;

namespace BBKeys
{
	const FName TargetActor = TEXT("TargetActor");
	const FName bIsPhase2 = TEXT("bIsPhase2");
	const FName bCanJumpAttack = TEXT("bCanJumpAttack");
	const FName bCanAttack = TEXT("bCanAttack");
}

UCLASS()
class VAMPIRELIKE3D_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ABossEnemy();

	UPROPERTY(BlueprintAssignable)
	FOnBossDied OnBossDied;

	UBehaviorTree* GetBehaviorTree() const;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackLeftMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackRightMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* SmashMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* JumpStartMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* JumpHoldMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* JumpEndMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* EnterPhase2Montage;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ShockwaveEffect;

	UFUNCTION()
	void DamageToPlayer(FVector TargetLocation, float Range);

	virtual void GetHit(float DamageAmount, const FVector& ImpactPoint) override;
protected:
	void Die() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float Phase2HpThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly)
	float Phase2SpeedMultiplier = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float Phase2AttackIntervalMultiplier = 0.6f;
private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	FTimerHandle AttackTimerHandle;
	FTimerHandle EffectTimerHandle;

	void EnterPhase2();
	bool bIsPhase2 = false;
	bool bIsCanJumpAttack = false;
	bool bIsLanded = false;
};

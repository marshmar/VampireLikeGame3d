#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_JumpAttack.generated.h"

class ABossEnemy;
class UParticleSystem;

UENUM()
enum class EJumpAttackState : uint8
{
	Roar,
	Jump
};

USTRUCT()
struct FJumpAttackTaskMemory
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ABossEnemy> Boss = nullptr;

	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;

	float ElapsedTime = 0.f;

	bool bHasDamaged = false;
	bool bPlayedLoopMontage = false;
	bool bPlayedSlamMontage = false;


	EJumpAttackState State = EJumpAttackState::Roar;
};

UCLASS()
class VAMPIRELIKE3D_API UBTTask_JumpAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_JumpAttack();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

private:
	UPROPERTY(EditAnywhere, Category = "Jump Attack")
	float JumpHeight = 800.f;

	UPROPERTY(EditAnywhere, Category = "Jump Attack")
	float TotalJumpTime = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Jump Attack")
	float DamageRadius = 250.f;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Effect")
	TObjectPtr<UParticleSystem> LandingEffect;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Effect")
	FVector LandingEffectScale = FVector(3.f);

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	TObjectPtr<UAnimMontage> RoarMontage;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	TObjectPtr<UAnimMontage> JumpStartMontage;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	TObjectPtr<UAnimMontage> JumpLoopMontage;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	TObjectPtr<UAnimMontage> JumpSlamMontage;
	
	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	float LoopMontageStartAlpha = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Jump Attack|Animation")
	float SlamMontageStartAlpha = 0.75f;


};

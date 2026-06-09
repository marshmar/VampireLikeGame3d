// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AutoAreaAttack.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API UBTTask_AutoAreaAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AutoAreaAttack();

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	UMaterialInterface* WarningDecalMaterial;

	UPROPERTY(EditAnywhere)
	float WarningDuration = 1.0f;   // 경고 원 유지 시간

	UPROPERTY(EditAnywhere)
	float WarningRadius = 200.f;   // 원 크기

	UPROPERTY(EditAnywhere)
	UParticleSystem* AttackEffect;

	float FillAmount;
	bool bIsEffectSpawned;

	FTimerHandle FillTimerHandle;
	FTimerHandle HitTimerHandle;
};

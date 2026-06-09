#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

class ABossEnemy;
/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	//void OnMontageEnded(UBehaviorTreeComponent* OwnerComp);
	void SpawnShockwave(ABossEnemy* MyClass, AActor* Target);
};

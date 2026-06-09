#pragma once

#include "CoreMinimal.h"
#include "Entities/Enemies/EnemyAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API ABossAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

	virtual void UpdateTarget(APawn* NewCharacter) override;
	virtual void Tick(float DeltaTime) override;
	void StartBehaviorTree();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BlackBoardComp;
};

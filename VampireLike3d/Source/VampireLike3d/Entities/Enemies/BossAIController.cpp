#include "Entities/Enemies/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossEnemy.h"
ABossAIController::ABossAIController()
{

}

void ABossAIController::UpdateTarget(APawn* NewCharacter)
{
	Super::UpdateTarget(NewCharacter);

	if (BlackBoardComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Blackboard is nullptr"));
		return;
	}
	
	BlackBoardComp->SetValueAsObject(TEXT("TargetActor"), GetPlayerPawn());
}

void ABossAIController::Tick(float DeltaTime)
{
	
}

void ABossAIController::StartBehaviorTree()
{
	ABossEnemy* BossClass = Cast<ABossEnemy>(GetPawn());
	if (!IsValid(BossClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Convert to BossClass"));
		return;

	}

	BehaviorTree = BossClass->GetBehaviorTree();
	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Behaviour Tree didn't set up"));
		return;
	}

	RunBehaviorTree(BehaviorTree);
	BlackBoardComp = GetBlackboardComponent();
	BlackBoardComp->SetValueAsObject(TEXT("TargetActor"), GetPlayerPawn());
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}




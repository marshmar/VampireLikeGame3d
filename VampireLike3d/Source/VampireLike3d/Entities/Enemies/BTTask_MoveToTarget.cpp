#include "Entities/Enemies/BTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h" 

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{

}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !BlackBoard)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to set up component"));
        return EBTNodeResult::Failed;
    }

    AActor* Target = Cast<AActor>(
        BlackBoard->GetValueAsObject(TEXT("TargetActor")));

    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("Target Actor is nullptr"));
        return EBTNodeResult::Failed;
    }

    Controller->SetFocus(Target);
    Controller->MoveToActor(Target, 300.f);

    UE_LOG(LogTemp, Warning, TEXT("Move To Target Succeeded"));
    return EBTNodeResult::Succeeded;
}
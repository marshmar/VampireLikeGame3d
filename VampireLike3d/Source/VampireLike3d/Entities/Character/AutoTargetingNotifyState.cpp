

#include "Entities/Character/AutoTargetingNotifyState.h"
#include "Interfaces/AutoTargetingInterface.h"

void UAutoTargetingNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AutoTargeter = Cast<IAutoTargetingInterface>(MeshComp->GetOwner());
	if (AutoTargeter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get AutoTargetingInterface"))
		return;
	}

	AutoTargeter->OnTargetingBegin();
}

void UAutoTargetingNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (AutoTargeter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoTargetingInterface is not setup"))
		return;
	}
	AutoTargeter->OnTargetingTick(FrameDeltaTime);
}

void UAutoTargetingNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AutoTargeter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AutoTargetingInterface is not setup"))
		return;
	}
	AutoTargeter->OnTargetingEnd();
}

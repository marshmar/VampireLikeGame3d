#include "Entities/Enemies/Bug.h"
#include "Components/BoxComponent.h"
#include "Utils/CollisionDefinitions.h"

ABug::ABug()
{
}

void ABug::BeginPlay()
{
	Super::BeginPlay();

	//UBoxComponent* OverlapBox = FindComponentByClass<UBoxComponent>();
	//if (OverlapBox)
	//{

	//	OverlapBox->SetGenerateOverlapEvents(true);
	//	OverlapBox->SetCollisionProfileName(TEXT("Custom"));

	//	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//	OverlapBox->SetCollisionObjectType(ECC_Enemy);

	//	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	//	OverlapBox->SetCollisionResponseToChannel(ECC_Player, ECR_Overlap);

	//	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::OnPlayerOverlap);
	//	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ABaseEnemy::OnPlayerOverlapEnd);

	//}
}

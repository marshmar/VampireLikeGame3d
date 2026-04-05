#pragma once

#include "CoreMinimal.h"
#include "GameFrameWork/Actor.h"
#include "Entities/Character/BaseCharacter.h" // TSubclassOf<ABaseCharacter> requires a full class definition, unlike raw pointers which allow forward declaration.
#include "PartyManager.generated.h"


class USwapGaugeComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSwapped, APawn*);

UCLASS()
class VAMPIRELIKE3D_API APartyManager : public AActor
{
	GENERATED_BODY()
	
public:
	/*
	* Delegate
	*/
	FOnPlayerSwapped OnPlayerSwapped;

	APartyManager();

	void SpawnPartyMembers();
	void AddPartyMember(TSubclassOf<ABaseCharacter> Character);

	void SwapCharacter(int32);
	void SwapCharacterToNext();
	void SwapCharacterToBef();

	ABaseCharacter* GetCurrentCharacter();

protected:
	virtual void BeginPlay() override;
private:
	USwapGaugeComponent* SwapGaugeComp;
	TArray<TSubclassOf<ABaseCharacter>> PartyMembers;
	TArray<ABaseCharacter*> SpawnedPartyMembers;
	int32 ActiveIndex;

	void DisableCharacter(ABaseCharacter* Character);
	void EnableCharacter(ABaseCharacter* Character);
};

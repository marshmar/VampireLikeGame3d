// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PartyManager.generated.h"

class ABaseCharacter;

UCLASS()
class VAMPIRELIKE3D_API UPartyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SpawnPartyMembers();
	void AddPartyMember(TSubclassOf<ABaseCharacter> Character);

	void SwapCharacter(int32);
	void SwapCharacterToNext();
	void SwapCharacterToBef();

	ABaseCharacter* GetCurrentCharacter();
private:
	TArray<TSubclassOf<ABaseCharacter>> PartyMembers;
	TArray<ABaseCharacter*> SpawnedPartyMembers;
	int32 ActiveIndex;

};

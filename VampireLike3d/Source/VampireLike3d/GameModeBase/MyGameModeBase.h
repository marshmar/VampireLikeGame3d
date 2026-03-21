// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/BaseCharacter.h"
#include "MyGameModeBase.generated.h"


UCLASS()
class VAMPIRELIKE3D_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Party")
	TSubclassOf<ABaseCharacter> CharacterClass1;

	UPROPERTY(EditDefaultsOnly, Category = "Party")
	TSubclassOf<ABaseCharacter> CharacterClass2;

	UPROPERTY(EditDefaultsOnly, Category = "Party")
	TSubclassOf<ABaseCharacter> CharacterClass3;
};

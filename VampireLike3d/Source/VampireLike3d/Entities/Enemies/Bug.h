// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Enemies/BaseEnemy.h"
#include "Bug.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRELIKE3D_API ABug : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ABug();
protected:
	virtual void BeginPlay() override;
};

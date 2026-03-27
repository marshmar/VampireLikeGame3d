#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Revenant.generated.h"


UCLASS()
class VAMPIRELIKE3D_API ARevenant : public ABaseCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void Attack() override;

private:
	UPROPERTY(EditAnyWhere, Category = "Effects")
	UParticleSystem* LaserEffect;
};

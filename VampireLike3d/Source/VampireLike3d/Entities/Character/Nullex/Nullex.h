#pragma once

#include "CoreMinimal.h"
#include "Entities/Character/BaseCharacter.h"
#include "Nullex.generated.h"

class AHomingProjectile;
class AGravityField;

UCLASS()
class VAMPIRELIKE3D_API ANullex : public ABaseCharacter
{

	GENERATED_BODY()
	
public:
	ANullex();


	virtual void BasicAttack() override;

	virtual void SwapAttack() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* BasicAttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AHomingProjectile> BasicAttackProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	TSubclassOf<AGravityField> GravityField;

	bool SpawnBasicAttackProjectile(AActor* Target, const FVector& SpawnLocation, const FRotator& SpawnRotation);
};

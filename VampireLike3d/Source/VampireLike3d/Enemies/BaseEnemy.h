#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "BaseEnemy.generated.h"

class UAnimMontage;
class UAttributeComponent;

UCLASS()
class VAMPIRELIKE3D_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* AttributeComp;

	virtual void BeginPlay() override;

	void Die(const FVector& ImpactPoint);
	float CalculateHitDegree(const FVector& ImpactPoint);
	void UpdateTarget(APawn* NewCharacter);
	/*
	* Play montage functions
	*/
	void PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage);


private:
	/*
	* Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float Health;	


};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/Poolable.h"
#include "Entities/Character/CharacterTypes.h"
#include "Entities/BaseEntity.h"
#include "BaseEnemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class ABaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, TSubclassOf<ABaseEnemy>, EnemyClass);

UCLASS()
class VAMPIRELIKE3D_API ABaseEnemy : public ABaseEntity, public IPoolable
{
	GENERATED_BODY()

public:
	/*
	* FrameWork
	*/
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnAcquired() override;
	virtual void OnReleased() override;

	/*
	* Hit
	*/
	virtual void GetHit(float DamageAmount, const FVector& ImpactPoint) override;
	virtual void Die() override;

	/*
	* Delegates
	*/
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDied OnEnemyDied;

	UFUNCTION()
	void OnHitReactEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnSpawnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/*
	* Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SpawnMontage;

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;
	void UpdateTarget(APawn* NewCharacter);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DeathEffect;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	float Health;	

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HomingTargetPoint;
	FTimerHandle HitReactTimerHandle;

	void HandleDeath();

	UFUNCTION()
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FTimerHandle DamageTimerHandle;
	ABaseCharacter* OverlappingPlayer = nullptr; // 데미지 줄 플레이어 저장

	void ApplyDamageToPlayer();
};

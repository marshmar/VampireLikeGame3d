#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityField.generated.h"

class UPhasedEffectComponent;

UCLASS()
class VAMPIRELIKE3D_API AGravityField : public AActor
{
	GENERATED_BODY()
	
public:	
	AGravityField();
	virtual void Tick(float DeltaTime) override;
	float GetIdleDuration() const;
	void SetIdleDuration(float Value);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float SlowSpeed = 3.0f;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float OriginSpeed;

	UPROPERTY(EditDefaultsOnly)
	UPhasedEffectComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void EndGravityField();
};

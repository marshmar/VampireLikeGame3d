#include "Entities/BaseEntity.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Entities/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
ABaseEntity::ABaseEntity()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false); // Capsule에도 설정 추
}

void ABaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEntity::GetHit(float DamageAmount, const FVector& ImpactPoint)
{
	if (!IsValid(AttributeComp))
	{
		UE_LOG(LogTemp, Error, TEXT("%s's Attribute Component is not set up"), *GetName());
		return;
	}

	AttributeComp->ReceiveDamage(CalculateFinalDamage(DamageAmount));

	if (AttributeComp->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		DirectionalDeath(ImpactPoint);
		Die();
	}
}

void ABaseEntity::DirectionalHitReact(const FVector& ImpactPoint)
{
	if (HitReactMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s's HitReactMontage didn't set up"), *GetName());
		return;
	}

	const float Theta = CalculateHitDegree(ImpactPoint);

	// Hit Direction Guide
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}
	PlayMontage(Section, HitReactMontage);

#pragma region Debug
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 300, 5.f, FColor::Blue, 5.f);
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	//}

	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 100, 5.f, FColor::Green, 5.f);
#pragma endregion
}

float ABaseEntity::CalculateHitDegree(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward| |TOHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void ABaseEntity::DirectionalDeath(const FVector& ImpactPoint)
{
	if (DeathMontage == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s's DeathMontage didn't set up"), *GetName());
		return;
	}

	const float Theta = CalculateHitDegree(ImpactPoint);
	FName Section("DeathBack");
	DeathPose = EDeathPose::EDP_DeathBack;
	if (Theta >= -90.f && Theta < 90.f)
	{
		Section = FName("DeathForward");
		DeathPose = EDeathPose::EDP_DeathForward;
	}
	PlayMontage(Section, DeathMontage);
}

void ABaseEntity::Die()
{
}

float ABaseEntity::CalculateFinalDamage(float DamageAmount)
{
	float BaseDefense = AttributeComp->GetArmor();

	float DamageReduction = 100.f / (100.f + BaseDefense);
	float FinalDamage = DamageAmount * DamageReduction;

	return FMath::Max(FinalDamage, 1.f); // 최소 1 보장
}

void ABaseEntity::InitializeStatus()
{
	// TODO: 데이터 테이블 만들어서 능력치 복사하기
}



void ABaseEntity::PlayMontage(const FName& SectionName, UAnimMontage* AnimMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find %s's AnimInstance"), *GetName());
		return;
	}

	AnimInstance->Montage_Play(AnimMontage);
	AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
}

void ABaseEntity::SpawnEffectAtLocation(UParticleSystem* Effect, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (Effect == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: try to spawn effect, but failed because the %s Effect is nullptr"), *GetName(), *Effect->GetName())
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		Effect,
		SpawnLocation,
		SpawnRotation
	);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/Poolable.h"
#include "PoolManagerSubsystem.generated.h"

USTRUCT()
struct FActorPool
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> AvailableActors;

	UPROPERTY()
	TArray<AActor*> ActiveActors;

	int32 MaxSize = 100;
};

UCLASS()
class VAMPIRELIKE3D_API UPoolManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	template<typename T>
	void InitializePool(int32 PoolSize);

	template<typename T>
	T* AcquireActor(const FTransform& SpawnTransform);

	template<typename T>
	T* AcquireActorDeferred(const FTransform& SpawnTransform);

	template<typename T>
	void ReleaseActor(T* Actor);

	template<typename T>
	void FinishAcquire(T* Actor);

	void InitializePoolByClass(TSubclassOf<AActor> ActorClass, int32 PoolSize);
	AActor* AcquireActorByClass(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform);
	void ReleaseActorByClass(AActor* Actor);
private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPool> Pools;

};

template<typename T>
void UPoolManagerSubsystem::InitializePool(int32 PoolSize)
{
	static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must derive from AActor");
	static_assert(TIsDerivedFrom<T, IPoolable>::IsDerived, "T must implement IPoolable");

	FActorPool& Pool = Pools.FindOrAdd(T::StaticClass());
	Pool.MaxSize = PoolSize;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < PoolSize; i++)
	{
		T* Actor = GetWorld()->SpawnActor<T>(T::StaticClass(), FTransform::Identity, Params);

		if (!IsValid(Actor))
		{
			continue;
		}

		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
		Actor->SetActorTickEnabled(false);
		Pool.AvailableActors.Add(Actor);
	}
}

template<typename T>
T* UPoolManagerSubsystem::AcquireActor(const FTransform& SpawnTransform)
{
	static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must derive from AActor");
	static_assert(TIsDerivedFrom<T, IPoolable>::IsDerived, "T must implement IPoolable");

	FActorPool* Pool = Pools.Find(T::StaticClass());

	// 풀에 여유가 없으면 동적 생성
	if (Pool == nullptr || Pool->AvailableActors.IsEmpty())
	{
		T* NewActor = GetWorld()->SpawnActor<T>(T::StaticClass(), SpawnTransform);
		return NewActor;
	}

	AActor* RawActor = Pool->AvailableActors.Pop();
	T* Actor = Cast<T>(RawActor);

	if (IsValid(Actor))
	{
		UE_LOG(LogTemp, Log, TEXT("spawn correct"))
		Pool->ActiveActors.Add(Actor);

		Actor->SetActorTransform(SpawnTransform);
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);

		Actor->OnAcquired();
	}
	
	return Actor;
}

template<typename T>
T* UPoolManagerSubsystem::AcquireActorDeferred(const FTransform& SpawnTransform)
{
	static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must derive from AActor");
	static_assert(TIsDerivedFrom<T, IPoolable>::IsDerived, "T must implement IPoolable");

	FActorPool* Pool = Pools.Find(T::StaticClass());

	AActor* RawActor = nullptr;

	if (Pool == nullptr || Pool->AvailableActors.IsEmpty())
	{
		RawActor = GetWorld()->SpawnActor<T>(T::StaticClass(), SpawnTransform);
	}
	else
	{
		RawActor = Pool->AvailableActors.Pop();
		Pool->ActiveActors.Add(RawActor);
	}

	T* Actor = Cast<T>(RawActor);

	if (IsValid(Actor))
	{
		Actor->SetActorTransform(SpawnTransform);

		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
		Actor->SetActorTickEnabled(false);
	}

	return Actor;
}

template<typename T>
void UPoolManagerSubsystem::ReleaseActor(T* Actor)
{
	static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must derive from AActor");
	static_assert(TIsDerivedFrom<T, IPoolable>::IsDerived, "T must implement IPoolable");

	if (Actor == nullptr)
	{
		return;
	}

	FActorPool* Pool = Pools.Find(T::StaticClass());
	if (Pool == nullptr)
	{
		return;
	}

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
	Actor->OnReleased();

	Pool->ActiveActors.Remove(Actor);
	Pool->AvailableActors.Add(Actor);
}

template<typename T>
void UPoolManagerSubsystem::FinishAcquire(T* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);

	Actor->OnAcquired();
}


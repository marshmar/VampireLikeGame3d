// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/ObjectPool/PoolManagerSubsystem.h"

void UPoolManagerSubsystem::InitializePoolByClass(TSubclassOf<AActor> ActorClass, int32 PoolSize)
{
    if (!ActorClass) return;

    FActorPool& Pool = Pools.FindOrAdd(ActorClass);
    Pool.MaxSize = PoolSize;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 i = 0; i < PoolSize; i++)
    {
        AActor* Actor = GetWorld()->SpawnActor<AActor>(ActorClass, FTransform::Identity, Params);
        if (!IsValid(Actor)) continue;

        Actor->SetActorHiddenInGame(true);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorTickEnabled(false);
        Pool.AvailableActors.Add(Actor);
    }
}

AActor* UPoolManagerSubsystem::AcquireActorByClass(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform)
{
    if (!ActorClass) return nullptr;

    FActorPool* Pool = Pools.Find(ActorClass);

    if (Pool == nullptr || Pool->AvailableActors.IsEmpty())
    {
        return GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform);
    }

    AActor* Actor = Pool->AvailableActors.Pop();
    if (IsValid(Actor))
    {
        Pool->ActiveActors.Add(Actor);
        Actor->SetActorTransform(SpawnTransform);
        Actor->SetActorHiddenInGame(false);
        Actor->SetActorEnableCollision(true);
        Actor->SetActorTickEnabled(true);

        IPoolable* Poolable = Cast<IPoolable>(Actor);
        if (Poolable)
        {
            Poolable->OnAcquired();
        }
    }

    return Actor;
}

void UPoolManagerSubsystem::ReleaseActorByClass(AActor* Actor)
{
    if (!IsValid(Actor)) return;

    FActorPool* Pool = Pools.Find(Actor->GetClass());
    if (Pool == nullptr) return;

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    IPoolable* Poolable = Cast<IPoolable>(Actor);
    if (Poolable)
    {
        Poolable->OnReleased();
    }

    Pool->ActiveActors.Remove(Actor);
    Pool->AvailableActors.Add(Actor);
}
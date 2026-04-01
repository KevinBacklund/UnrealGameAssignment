// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "MyGameMode.h"

void UEnemyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(MyLog, Warning, TEXT("EnemyManager Initialized"));
}

void UEnemyManager::Deinitialize()
{
	UE_LOG(MyLog, Warning, TEXT("EnemyManager Deinitialized"));
	Super::Deinitialize();
}

void UEnemyManager::AddEnemy(class AEnemy* Enemy)
{
	Enemies.Add(Enemy);
}

void UEnemyManager::RemoveEnemy(class AEnemy* Enemy)
{
	Enemies.Remove(Enemy);
}

TArray<class AEnemy*> UEnemyManager::GetEnemies() const
{
	return Enemies;
}
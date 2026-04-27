// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
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

void UEnemyManager::SpawnNextWave(TArray<TSubclassOf<AEnemy>> EnemiesToSpawn, FVector Location, float SpawnInterval)
{
	CurrentWave++;
	UE_LOG(MyLog, Warning, TEXT("Spawning Enemy in Wave %d"), CurrentWave);
	for (int i = 0; i < EnemiesToSpawn.Num(); i++)
	{
		FTimerHandle SpawnTimer;
		FVector SpawnLocation = FMath::VRand() * 200.0f + Location;
		SpawnLocation.Z = Location.Z;
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateUObject(this, &UEnemyManager::SpawnEnemy, EnemiesToSpawn[i], SpawnLocation), SpawnInterval * i, false);
	}
}

void UEnemyManager::SpawnEnemy(TSubclassOf<AEnemy> EnemyClass, FVector Location)
{
	AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, Location, FRotator::ZeroRotator);
	if (SpawnedEnemy)
	{
		AddEnemy(SpawnedEnemy);
	}
}

void UEnemyManager::Reset()
{
	ClearTimers();
	for (AEnemy* Enemy : Enemies)
	{
		if (Enemy)
		{
			Enemy->Destroy();
		}
	}
	Enemies.Empty();
	CurrentWave = 0;
}

void UEnemyManager::ClearTimers()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearAllTimersForObject(this);
}
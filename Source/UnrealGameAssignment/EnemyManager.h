// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEASSIGNMENT_API UEnemyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	void AddEnemy(class AEnemy* Enemy);
	void RemoveEnemy(class AEnemy* Enemy);
	UFUNCTION(BlueprintCallable)
	TArray<class AEnemy*> GetEnemies() const;
	UFUNCTION(BlueprintCallable)
	void SpawnNextWave(TArray<TSubclassOf<AEnemy>> EnemiesToSpawn, FVector Location, float SpawnInterval);

	UFUNCTION(BlueprintCallable)
	void Reset();

private:
	TArray<class AEnemy*> Enemies;
	void SpawnEnemy(TSubclassOf<AEnemy> EnemyClass, FVector Location);
	void ClearTimers();

public:
	UPROPERTY(BlueprintReadOnly)
	int CurrentWave = 0;
};

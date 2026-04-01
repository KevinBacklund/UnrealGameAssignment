// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class UNREALGAMEASSIGNMENT_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void FireAtTargetEnemy();
	class AEnemy* FindTargetEnemy();

private:
	float FireRate = 1.0f;
	float FireRange = 1000.0f; 
	FTimerHandle FireTimerHandle;
	class AEnemy* TargetEnemy;
};

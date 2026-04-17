// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Enemy.h"	
#include "EnemyManager.h"
#include "MyGameMode.h"
#include "Projectile.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuildCost = 20;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{

		FindTargetEnemy();
		
		if (TargetEnemy)
		{
			FireAtTargetEnemy(TargetEnemy->GetActorLocation());
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireRate, false);
		}
	}
}

void ATurret::FindTargetEnemy()
{
	TArray<AEnemy*> Enemies = GetWorld()->GetGameInstance()->GetSubsystem<UEnemyManager>()->GetEnemies();
	AEnemy* ClosestEnemy = nullptr;
	float ClosestDistance = FireRange;
	for (AEnemy* Enemy : Enemies)
	{
		float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestEnemy = Enemy;
		}
	}
	TargetEnemy = ClosestEnemy;
}

void ATurret::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
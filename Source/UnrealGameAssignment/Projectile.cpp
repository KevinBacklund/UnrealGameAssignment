// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = 10.0f;
	Speed = 1000.0f;
	Target = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), Target, DeltaTime, Speed));
	if (FVector::Dist(Target, GetActorLocation()) < 10)
	{
		Destroy();
	}
}

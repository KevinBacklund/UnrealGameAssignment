// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceExtractor.h"

AResourceExtractor::AResourceExtractor()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildCost = 50.0f;
	NeedsResource = true;
	HasInventory = true;
	ExtractionRate = 1.0f;
	ExtractionTimer = 0.0f;
	InventoryCapacity = 5;
}

void AResourceExtractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ExtractionTimer += DeltaTime;
	if (ExtractionTimer >= 1.0f / ExtractionRate)
	{
		ExtractionTimer = 0.0f;
		if(!InventoryFull)
		{
			FVector SpawnLocation = GetActorLocation() - FVector(0.0f, 0.0f, 38.0f);
			AddItem(GetWorld()->SpawnActor<AActor>(ResourceClass, SpawnLocation, FRotator::ZeroRotator));
		}
	}
}
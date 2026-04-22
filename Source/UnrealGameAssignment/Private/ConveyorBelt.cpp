// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"

AConveyorBelt::AConveyorBelt()
{
	PrimaryActorTick.bCanEverTick = true;
	Speed = 200.0f;
	HasInventory = true;
	InventoryCapacity = 1;
	DestinationBuilding = nullptr;
	SourceBuilding = nullptr;
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	DestinationBuilding = FindConnectedBuilding(GetActorForwardVector());
	SourceBuilding = FindConnectedBuilding(-GetActorForwardVector());
	if (Cast<AConveyorBelt>(DestinationBuilding)) 
	{
		Cast<AConveyorBelt>(DestinationBuilding)->SourceBuilding = this;
	}
	if (Cast<AConveyorBelt>(SourceBuilding)) 
	{
		Cast<AConveyorBelt>(SourceBuilding)->DestinationBuilding = this;
	}
}

void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABuilding* AConveyorBelt::FindConnectedBuilding(FVector Direction)
{
	FVector Start = GetActorLocation() + Direction * 100.0f;
	FVector End = GetActorLocation() + Direction * 200.0f;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, Params);
	for (const FHitResult& Hit : HitResults)
	{
		ABuilding* Building = Cast<ABuilding>(Hit.GetActor());
		if (Building)
		{
			return Building;
		}
	}
	return nullptr;
}

void AConveyorBelt::MoveItem(float DeltaTime)
{
	if (!InventoryEmpty() && DestinationBuilding && !DestinationBuilding->InventoryFull)
	{
		AActor* Item = RemoveItem();
		FVector Direction = (DestinationBuilding->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = Item->GetActorLocation() + Direction * Speed * DeltaTime;
		Item->SetActorLocation(NewLocation);
		if (FVector::Dist(NewLocation, DestinationBuilding->GetActorLocation()) < 100.0f)
		{
			DestinationBuilding->AddItem(Item);
		}
	}
	if (SourceBuilding && !SourceBuilding->InventoryEmpty() && !InventoryFull && !Cast<AConveyorBelt>(SourceBuilding))
	{
		AActor* Item = SourceBuilding->RemoveItem();
		AddItem(Item);
	}
}
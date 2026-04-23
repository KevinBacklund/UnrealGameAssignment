// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"
#include "MyPlayerController.h"
#include "MyGameMode.h"

AConveyorBelt::AConveyorBelt()
{
	PrimaryActorTick.bCanEverTick = true;
	Speed = 200.0f;
	HasInventory = true;
	Directional = true;
	InventoryCapacity = 1;
	DestinationBuilding = nullptr;
	SourceBuilding = nullptr;
	CurrentItem = nullptr;
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	DestinationBuilding = FindConnectedBuilding(GetActorForwardVector());
	SourceBuilding = FindConnectedBuilding(-GetActorForwardVector());
	if (!DestinationBuilding || !SourceBuilding)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OnBuildingPlaced.AddDynamic(this, &AConveyorBelt::OnBuildingPlaced);
		}
	}
}

void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveItem(DeltaTime);
}

ABuilding* AConveyorBelt::FindConnectedBuilding(FVector Direction)
{
	FVector Start = MeshComponent->GetComponentLocation() + Direction * 50.0f;
	FVector End = MeshComponent->GetComponentLocation() + Direction * 100.0f;
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
	if (CurrentItem)
	{
		FVector TargetLocation;
		if (!DestinationBuilding || DestinationBuilding->InventoryFull)
		{
			TargetLocation = GetActorLocation();
		}
		else
		{
			TargetLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		}
		if (FVector::Dist(CurrentItem->GetActorLocation(), TargetLocation) < 10.0f)
		{
			if (DestinationBuilding && DestinationBuilding->AddItem(CurrentItem))
			{
				RemoveItem();
				CurrentItem = nullptr;
			}
		}
		else
		{
			CurrentItem->SetActorLocation(FMath::VInterpConstantTo(CurrentItem->GetActorLocation(), TargetLocation, DeltaTime, Speed));
		}
	}
	else if (SourceBuilding && !SourceBuilding->InventoryEmpty() && !Cast<AConveyorBelt>(SourceBuilding))
	{
		CurrentItem = SourceBuilding->RemoveItem();
	}
	else if (!InventoryEmpty())
	{
		CurrentItem = Inventory[0];
	}
}


void AConveyorBelt::OnBuildingPlaced(ABuilding* PlacedBuilding)
{
	if (!DestinationBuilding)
	{
		DestinationBuilding = FindConnectedBuilding(GetActorForwardVector());
	}
	if (!SourceBuilding)
	{
		SourceBuilding = FindConnectedBuilding(-GetActorForwardVector());
	}
	if (DestinationBuilding && SourceBuilding)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OnBuildingPlaced.RemoveDynamic(this, &AConveyorBelt::OnBuildingPlaced);
		}
	}
}
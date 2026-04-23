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
			UE_LOG(MyLog, Display, TEXT("Found Player Controller!!"));
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
	if(!InventoryFull && SourceBuilding && !SourceBuilding->InventoryEmpty() && !Cast<AConveyorBelt>(SourceBuilding))
	{
		CurrentItem = SourceBuilding->RemoveItem();
		AddItem(CurrentItem);
	}
	if (CurrentItem)
	{
		FVector TargetLocation;
		if (DestinationBuilding && !DestinationBuilding->InventoryFull)
		{
			TargetLocation = FVector(DestinationBuilding->GetActorLocation().X, DestinationBuilding->GetActorLocation().Y, CurrentItem->GetActorLocation().Z);
		}
		else 
		{
			TargetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, CurrentItem->GetActorLocation().Z);
		}
		CurrentItem->SetActorLocation(CurrentItem->GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
		if (FVector::Dist(CurrentItem->GetActorLocation(), TargetLocation) < 10.0f)
		{
			CurrentItem->SetActorLocation(TargetLocation);
			if (DestinationBuilding && !DestinationBuilding->InventoryFull)
			{
				DestinationBuilding->AddItem(CurrentItem);
				RemoveItem();
				CurrentItem = nullptr;
			}
		}
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
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
	UpdateConnectedBuildings();
}

void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveItem(DeltaTime);
}

void AConveyorBelt::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnBuildingPlaced.RemoveDynamic(this, &AConveyorBelt::OnBuildingPlaced);
		PlayerController->OnBuildingDeconstructed.RemoveDynamic(this, &AConveyorBelt::OnBuildingDeconstructed);
	}
	if (CurrentItem)
	{
		CurrentItem->Destroy();
		CurrentItem = nullptr;
	}
}

ABuilding* AConveyorBelt::FindConnectedBuilding(FVector Direction)
{
	FVector Start = MeshComponent->GetComponentLocation() + Direction * 50.0f;
	FVector End = MeshComponent->GetComponentLocation() + Direction * 120.0f;
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
		FVector DestinationLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;
		DestinationLocation.Z = CurrentItem->GetActorLocation().Z;
		FVector SelfLocation = GetActorLocation();
		SelfLocation.Z = CurrentItem->GetActorLocation().Z;
		if (DestinationBuilding && !DestinationBuilding->InventoryFull)
		{
			CurrentItem->SetActorLocation(FMath::VInterpConstantTo(CurrentItem->GetActorLocation(), DestinationLocation, DeltaTime, Speed));
		}
		if (!DestinationBuilding || DestinationBuilding->InventoryFull)
		{
			float DotProduct = FVector::DotProduct((CurrentItem->GetActorLocation() - SelfLocation).GetSafeNormal(), GetActorForwardVector());
			if (DotProduct < 0)
			{
				CurrentItem->SetActorLocation(FMath::VInterpConstantTo(CurrentItem->GetActorLocation(), SelfLocation, DeltaTime, Speed));
			}
		}
		if (FVector::Dist(CurrentItem->GetActorLocation(), DestinationLocation) < 10.0f)
		{
			if (DestinationBuilding && DestinationBuilding->AddItem(CurrentItem))
			{
				RemoveItem();
				CurrentItem = nullptr;
			}
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

void AConveyorBelt::OnBuildingDeconstructed()
{
	UpdateConnectedBuildings();
	if (!DestinationBuilding && !SourceBuilding)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OnBuildingDeconstructed.RemoveDynamic(this, &AConveyorBelt::OnBuildingDeconstructed);
		}
	}
}

void AConveyorBelt::UpdateConnectedBuildings()
{
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
	if (DestinationBuilding || SourceBuilding)
	{
		AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->OnBuildingDeconstructed.AddDynamic(this, &AConveyorBelt::OnBuildingDeconstructed);
		}
	}
}
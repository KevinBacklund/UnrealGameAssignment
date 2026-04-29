// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayerCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Turret.h"
#include "MyGameMode.h"
#include "ResourceExtractor.h"
#include "Engine/OverlapResult.h"
#include "ResourceNode.h"	
#include "BuildingGhost.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	PrimaryActorTick.bCanEverTick = true;
	GridSize = 100.0f;
	Resources = 500.0f;
	Ghost = nullptr;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
	InputComponent->BindAxis("Zoom", this, &AMyPlayerController::Zoom);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AMyPlayerController::PlaceBuilding);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AMyPlayerController::DeconstructBuilding);
	InputComponent->BindAction("Rotate", IE_Pressed, this, &AMyPlayerController::RotateBuilding);
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BuildingClass)
	{
		ShowBuildingGhost();
	}
}

void AMyPlayerController::MoveForward(float fValue)
{
	if (GetPawn() && fValue != 0.0f)
	{
		AMyPlayerCameraPawn* CameraPawn = Cast<AMyPlayerCameraPawn>(GetPawn());
		float Scale = CameraPawn->SpringArmComponent->TargetArmLength / 1000.0f;
		GetPawn()->AddActorWorldOffset(FVector::ForwardVector * fValue * 300.0f * Scale * GetWorld()->GetDeltaSeconds());
	}
}

void AMyPlayerController::MoveRight(float fValue)
{
	if (GetPawn() && fValue != 0.0f)
	{
		AMyPlayerCameraPawn* CameraPawn = Cast<AMyPlayerCameraPawn>(GetPawn());
		float Scale = CameraPawn->SpringArmComponent->TargetArmLength / 1000.0f;
		GetPawn()->AddActorWorldOffset(FVector::RightVector * fValue * 300.0f * Scale * GetWorld()->GetDeltaSeconds());
	}
}

void AMyPlayerController::Zoom(float fValue)
{
	if (GetPawn() && fValue != 0.0f)
	{
		AMyPlayerCameraPawn* CameraPawn = Cast<AMyPlayerCameraPawn>(GetPawn());
		if (CameraPawn)
		{
			CameraPawn->SpringArmComponent->TargetArmLength += fValue * 3000.0f * GetWorld()->GetDeltaSeconds();
			CameraPawn->SpringArmComponent->TargetArmLength = FMath::Clamp(CameraPawn->SpringArmComponent->TargetArmLength, 300.0f, 4000.0f);
		}
	}
}

void AMyPlayerController::AddResources(float Amount)
{
	Resources += Amount;
	OnResourceChanged.Broadcast(Resources);
}

void AMyPlayerController::RotateBuilding()
{
	BuildingRotation.Yaw += 90.0f;
	BuildingRotation.Yaw = FMath::Fmod(BuildingRotation.Yaw, 360.0f);
}

void AMyPlayerController::ClearBuildingSelection()
{
	BuildingClass = nullptr;
	if (Ghost)
	{
		Ghost->Destroy();
		Ghost = nullptr;
	}
}

void AMyPlayerController::ShowBuildingGhost()
{
	FHitResult HitResult;
	bool validLocation = true;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult) && BuildingClass)
	{
		float BuildCost = BuildingClass->GetDefaultObject<ABuilding>()->BuildCost;
		if (Resources < BuildCost)
		{
			validLocation = false;
		}
		FVector Location = HitResult.Location;
		Location.Z = 0.0f;
		Location = FVector(FMath::RoundToInt(Location.X / GridSize) * GridSize, FMath::RoundToInt(Location.Y / GridSize) * GridSize, 50.0f);
		if (BuildingClass->GetDefaultObject<ABuilding>()->NeedsResource)
		{
			TArray<FOverlapResult> Overlaps;
			GetWorld()->OverlapMultiByChannel(Overlaps, FVector(Location.X, Location.Y, 60.0f), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GridSize * 1.5));
			bool HasResourceNode = false;
			for (const FOverlapResult& Result : Overlaps)
			{
				if (Result.GetActor() && Result.GetActor()->IsA(AResourceNode::StaticClass()))
				{
					HasResourceNode = true;
					break;
				}
			}
			if (!HasResourceNode)
			{
				validLocation = false;
			}
		}
		if (GetWorld()->OverlapAnyTestByChannel(FVector(Location.X, Location.Y, 0), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(49.0f)))
		{
			validLocation = false;
		}
		if (!Ghost)
		{
			Ghost = Cast<ABuildingGhost>(GetWorld()->SpawnActor(ABuildingGhost::StaticClass(), &Location, &BuildingRotation));
		}
		if (Ghost)
		{
			UStaticMesh* Mesh = BuildingClass->GetDefaultObject<ABuilding>()->MeshComponent->GetStaticMesh();
			Ghost->SetMesh(Mesh);
			Ghost->ValidPosition(validLocation);
			Ghost->SetActorLocation(Location);
			Ghost->SetActorRotation(BuildingRotation);
			Ghost->StaticMeshComponent->SetWorldScale3D(BuildingClass->GetDefaultObject<ABuilding>()->MeshComponent->GetRelativeScale3D());
			Ghost->StaticMeshComponent->SetRelativeLocation(BuildingClass->GetDefaultObject<ABuilding>()->MeshComponent->GetRelativeLocation());
			if (BuildingClass->GetDefaultObject<ABuilding>()->Directional)
			{
				Ghost->ShowDirectionPointer(true);
			}
			else
			{
				Ghost->ShowDirectionPointer(false);
			}
		}
	}
}

void AMyPlayerController::PlaceBuilding()
{
	FHitResult HitResult;
	if(GetHitResultUnderCursor(ECC_Visibility, false, HitResult) && BuildingClass)
	{
		float BuildCost = BuildingClass->GetDefaultObject<ABuilding>()->BuildCost;
		if (Resources < BuildCost)
		{
			UE_LOG(MyLog, Warning, TEXT("Not enough resources to place building!"));
			return;
		}
		FVector Location = HitResult.Location;
		Location = FVector(FMath::RoundToInt(Location.X / GridSize) * GridSize, FMath::RoundToInt(Location.Y / GridSize) * GridSize, 49.0f);
		if (BuildingClass->GetDefaultObject<ABuilding>()->NeedsResource) 
		{
			TArray<FOverlapResult> Overlaps;
			GetWorld()->OverlapMultiByChannel(Overlaps, FVector(Location.X, Location.Y, 60.0f), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(GridSize*1.5));
			bool HasResourceNode = false;
			for (const FOverlapResult& Result : Overlaps)
			{
				if (Result.GetActor() && Result.GetActor()->IsA(AResourceNode::StaticClass()))
				{
					HasResourceNode = true;
					break;
				}
			}
			if (!HasResourceNode)
			{
				UE_LOG(MyLog, Warning, TEXT("Building must be placed near a resource node!"));
				return;
			}
		}

		if (GetWorld()->OverlapAnyTestByChannel(FVector(Location.X, Location.Y, 0), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(49.0f)))
		{
			UE_LOG(MyLog, Warning, TEXT("Cannot place building here!"));
			return;
		}
		ABuilding* PlacedBuilding = GetWorld()->SpawnActor<ABuilding>(BuildingClass, Location, BuildingRotation);
		OnBuildingPlaced.Broadcast(PlacedBuilding);
		Resources -= BuildCost;
		OnResourceChanged.Broadcast(Resources);
		UE_LOG(MyLog, Display, TEXT("BUILDING SPAWNED"));
	}
}

void AMyPlayerController::DeconstructBuilding()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		FVector Location = HitResult.Location;
		Location.Z = 0.0f;
		Location = FVector(FMath::RoundToInt(Location.X / GridSize) * GridSize, FMath::RoundToInt(Location.Y / GridSize) * GridSize, 50.0f);
		TArray<FOverlapResult> Overlaps;
		GetWorld()->OverlapMultiByChannel(Overlaps, FVector(Location.X, Location.Y, 50.0f), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(49.0f));
		UE_LOG(MyLog, Display, TEXT("OVERLAPS FOUND: %d"), Overlaps.Num());
		for (const FOverlapResult& Result : Overlaps)
		{
			ABuilding* Building = Cast<ABuilding>(Result.GetActor());
			if (Building && Building->IsDeconstructable)
			{
				float RefundAmount = Building->BuildCost * 0.5f;
				AddResources(RefundAmount);
				Building->Destroy();
				OnBuildingDeconstructed.Broadcast();
				UE_LOG(MyLog, Display, TEXT("BUILDING DECONSTRUCTED"));
				break;
			}
		}
	}
}
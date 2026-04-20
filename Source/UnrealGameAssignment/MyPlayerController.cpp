// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayerCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Turret.h"
#include "MyGameMode.h"
#include "ResourceExtractor.h"
#include "Engine/OverlapResult.h"
#include "ResourceNode.h"	

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::MoveRight);
	InputComponent->BindAxis("Zoom", this, &AMyPlayerController::Zoom);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AMyPlayerController::PlaceBuilding);
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
		Location.Z = 0.0f;
		float GridSize = 200.0f;
		Location = FVector(FMath::RoundToInt(Location.X / GridSize) * GridSize, FMath::RoundToInt(Location.Y / GridSize) * GridSize, 0.0f);
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
		if (GetWorld()->OverlapAnyTestByChannel(FVector(Location.X, Location.Y, 60.0f), FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.0f)))
		{
			UE_LOG(MyLog, Warning, TEXT("Cannot place building here!"));
			return;
		}
		GetWorld()->SpawnActor<ABuilding>(BuildingClass, Location, FRotator::ZeroRotator);
		Resources -= BuildCost;
		OnResourceChanged.Broadcast(Resources);
		UE_LOG(MyLog, Display, TEXT("BUILDING SPAWNED"));
	}
}
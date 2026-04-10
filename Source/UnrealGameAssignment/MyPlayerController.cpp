// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPlayerCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Turret.h"
#include "MyGameMode.h"

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
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AMyPlayerController::PlaceTurret);
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
			CameraPawn->SpringArmComponent->TargetArmLength = FMath::Clamp(CameraPawn->SpringArmComponent->TargetArmLength, 300.0f, 2000.0f);
		}
	}
}

void AMyPlayerController::PlaceTurret()
{
	FHitResult HitResult;
	if(GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		UE_LOG(MyLog, Display, TEXT("TURRETSPAWN"));
		FVector Location = HitResult.Location;
		Location.Z = 0.0f;
		GetWorld()->SpawnActor<ATurret>(TurretClass, Location, FRotator::ZeroRotator);
	}
}
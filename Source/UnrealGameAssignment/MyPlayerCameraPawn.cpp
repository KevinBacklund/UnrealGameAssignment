// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCameraPawn.h"
#include "MyGameMode.h"

// Sets default values
AMyPlayerCameraPawn::AMyPlayerCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMyPlayerCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayerCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayerCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayerCameraPawn::MoveRight);
}

void AMyPlayerCameraPawn::MoveForward(float fValue)
{
	if (Controller && fValue != 0.0f)
	{
		this->SetActorLocation(GetActorLocation() + FVector::ForwardVector * fValue * movementSpeed * GetWorld()->GetDeltaSeconds());
	}
}

void AMyPlayerCameraPawn::MoveRight(float fValue)
{
	if (Controller && fValue != 0.0f)
	{
		this->SetActorLocation(GetActorLocation() + FVector::RightVector * fValue * movementSpeed * GetWorld()->GetDeltaSeconds());
	}
}
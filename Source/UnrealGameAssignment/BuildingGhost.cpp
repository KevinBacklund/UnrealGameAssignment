// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGhost.h"

// Sets default values
ABuildingGhost::ABuildingGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	DirectionPointerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DirectionPointerMesh"));
	DirectionPointerMesh->SetupAttachment(StaticMeshComponent);
	DirectionPointerMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone")));	
	DirectionPointerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DirectionPointerMesh->SetVisibility(false);
	DirectionPointerMesh->SetRelativeLocation(FVector(GetActorForwardVector() * 100.0f + FVector(0.0f, 0.0f, 20.0f)));
	DirectionPointerMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 90.0f));
	DirectionPointerMesh->SetRelativeScale3D(FVector(0.5f, 0.2f, 0.5f));

	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/M_GhostMat.M_GhostMat"));
	if (Material)
	{
		StaticMeshComponent->SetMaterial(0, Material);
		DirectionPointerMesh->SetMaterial(0, Material);
	}
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ABuildingGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingGhost::ValidPosition(bool IsValid)
{
	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		UMaterialInstanceDynamic* DirectionPointerDynamicMaterial = DirectionPointerMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("Colour"), IsValid ? FLinearColor::Green : FLinearColor::Red);
		}
		if (DirectionPointerDynamicMaterial)
		{
			DirectionPointerDynamicMaterial->SetVectorParameterValue(TEXT("Colour"), IsValid ? FLinearColor::Green : FLinearColor::Red);
		}
	}
}

void ABuildingGhost::SetMesh(UStaticMesh* Mesh)
{
	if (StaticMeshComponent && Mesh)
	{
		StaticMeshComponent->SetStaticMesh(Mesh);
	}
}
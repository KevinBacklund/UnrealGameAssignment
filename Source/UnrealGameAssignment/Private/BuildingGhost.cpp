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

	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/M_GhostMat.M_GhostMat"));
	if (Material)
	{
		StaticMeshComponent->SetMaterial(0, Material);
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
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("Color"), IsValid ? FLinearColor::Green : FLinearColor::Red);
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
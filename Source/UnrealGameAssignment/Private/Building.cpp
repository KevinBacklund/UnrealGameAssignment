// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NeedsResource = false;
	HasInventory = false;
	InventoryFull = false;
	InventoryCapacity = 10;
	ItemCount = 0;
	BuildCost = 10.0f;
	Inventory = TArray<AActor*>();

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::AddItem(AActor* Item)
{
	if (ItemCount < InventoryCapacity)
	{
		Inventory.Push(Item);
		ItemCount++;
		if (ItemCount == InventoryCapacity)
		{
			InventoryFull = true;
		}
	}
}

AActor* ABuilding::RemoveItem()
{
	if (ItemCount > 0)
	{
		AActor* Item = Inventory.Pop();
		ItemCount--;
		InventoryFull = false;
		return Item;
	}
	return nullptr;
}
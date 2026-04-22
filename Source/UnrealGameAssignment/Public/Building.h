// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class UNREALGAMEASSIGNMENT_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float BuildCost;
	bool NeedsResource;
	bool HasInventory;
	bool InventoryFull;
	bool InventoryEmpty() { return ItemCount == 0; }
	int InventoryCapacity;

	void AddItem(AActor* Item);
	AActor* RemoveItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UStaticMeshComponent* MeshComponent;

private:
	TArray<AActor*> Inventory;
	int ItemCount;
};

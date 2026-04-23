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
	TArray<AActor*> Inventory;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float BuildCost;
	bool NeedsResource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	bool HasInventory;
	bool InventoryFull;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	bool Directional;
	UFUNCTION(BlueprintCallable, Category = "Building")
	bool InventoryEmpty() { return ItemCount == 0; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	int InventoryCapacity;

	AActor* AddItem(AActor* Item);
	UFUNCTION(BlueprintCallable, Category = "Building")
	AActor* RemoveItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UStaticMeshComponent* MeshComponent;

private:
	int ItemCount;
};

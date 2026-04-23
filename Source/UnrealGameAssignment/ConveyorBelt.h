// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "ConveyorBelt.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEASSIGNMENT_API AConveyorBelt : public ABuilding
{
	GENERATED_BODY()
	
public:
	AConveyorBelt();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ConveyorBelt")
	float Speed;

	ABuilding* DestinationBuilding;
	ABuilding* SourceBuilding;

private:
	ABuilding* FindConnectedBuilding(FVector Direction);
	void MoveItem(float DeltaTime);
	AActor* CurrentItem;

	void OnBuildingPlaced(ABuilding* PlacedBuilding);
};

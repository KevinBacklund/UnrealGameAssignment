// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingGhost.generated.h"

UCLASS()
class UNREALGAMEASSIGNMENT_API ABuildingGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UStaticMeshComponent* DirectionPointerMesh;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* Material;

	void ValidPosition(bool IsValid);
	void SetMesh(UStaticMesh* Mesh);
	void ShowDirectionPointer(bool Show) { if (DirectionPointerMesh) DirectionPointerMesh->SetVisibility(Show); }
};

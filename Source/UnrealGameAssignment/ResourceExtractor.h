// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building.h"
#include "ResourceExtractor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEASSIGNMENT_API AResourceExtractor : public ABuilding
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AResourceExtractor();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Extraction")
	float ExtractionRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Extraction")
	TSubclassOf<class AActor> ResourceClass;
private:
	float ExtractionTimer;
};

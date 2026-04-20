// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Turret.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceChanged, float, CurrentResources);

UCLASS()
class UNREALGAMEASSIGNMENT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	virtual void SetupInputComponent() override;


private:
	void MoveForward(float fValue);
	void MoveRight(float fValue);
	void Zoom(float fValue);
	void PlaceBuilding();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABuilding> BuildingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resources;

	UPROPERTY(BlueprintAssignable)
	FOnResourceChanged OnResourceChanged;

	UFUNCTION(BlueprintCallable)
	void AddResources(float Amount);
};

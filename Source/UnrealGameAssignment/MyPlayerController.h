// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Turret.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
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
	void PlaceTurret();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATurret> TurretClass;
};

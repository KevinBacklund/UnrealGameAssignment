// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(MyLog, Log, All)
UCLASS()
class UNREALGAMEASSIGNMENT_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
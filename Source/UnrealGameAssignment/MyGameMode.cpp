// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
DEFINE_LOG_CATEGORY(MyLog);
void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(MyLog, Warning, TEXT("GameMode BeginPlay"));
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyManager.h"
#include "HealthComponent.h"
#include "MyGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetLocation = FVector::ZeroVector;
	Health = 100.0f;
	Speed = 300.0f;
	Damage = 10.0f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetGameInstance()->GetSubsystem<UEnemyManager>()->AddEnemy(this);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, Speed));
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetGameInstance()->GetSubsystem<UEnemyManager>()->RemoveEnemy(this);
	Super::EndPlay(EndPlayReason);
}

void AEnemy::SetTarget(AActor* Target)
{
	if (Target)
	{
		TargetActor = Target;
		TargetLocation = TargetActor->GetActorLocation();
		if (TargetActor->GetComponentByClass(UHealthComponent::StaticClass()))
		{
			UHealthComponent* HealthComponent = Cast<UHealthComponent>(TargetActor->GetComponentByClass(UHealthComponent::StaticClass()));
			if (HealthComponent)
			{
				HealthComponent->OnDeath.AddDynamic(this, &AEnemy::OnTargetDeath);
			}
		}
	}
}

void AEnemy::OnTargetDeath()
{
	TargetActor = nullptr;
	TargetLocation = FVector::ZeroVector;
}
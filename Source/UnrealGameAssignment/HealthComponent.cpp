// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{


}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}

void UHealthComponent::Heal(float HealAmount)
{
	CurrentHealth += HealAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}

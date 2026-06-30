// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetHealth();
	// ...
	
}

void UHealthComponent::ResetHealth()
{
	CurrentHP = MaxHP;
	bIsDead = false;
}

void UHealthComponent::ApplyDamage(int32 Damage)
{
	if (bIsDead) return;	
	CurrentHP = FMath::Max(0, CurrentHP - Damage);	
}

void UHealthComponent::MarkDead()
{
	bIsDead = true;
}

float UHealthComponent::GetHealthPercent() const
{
	return MaxHP > 0 ? (float)CurrentHP / static_cast<float>(MaxHP) : 0.0f;
}


// Called every frame
/*void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/


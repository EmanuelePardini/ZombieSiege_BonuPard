// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "Character/SurvivorCharacter.h"
#include "Character/SurvivorController.h"
#include "Interfaces/SpawnInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::SetupInitialHealth()
{
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	OnHealthChanged.Broadcast(Health);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DecrementHealth(float DamageAmount)
{
	Health -= DamageAmount;
	if(Health <= 0)
	{
		Die();
	}
		OnHealthChanged.Broadcast(Health);
}

void UHealthComponent::IncrementHealth(float RecoverAmount)
{
	//Add recover amount if not maximum
	if(Health < MaxHealth)
	{
		Health += RecoverAmount;
	}
	//If after adding recover amount it overflow reassign max value
	if(Health >= MaxHealth)
	{
		Health = MaxHealth;
	}
		OnHealthChanged.Broadcast(Health);
}

void UHealthComponent::Die()
{
	ISpawnInterface* DieActor = Cast<ISpawnInterface>(GetOwner());
	if (DieActor)
	{
		DieActor->Die();
	}
}


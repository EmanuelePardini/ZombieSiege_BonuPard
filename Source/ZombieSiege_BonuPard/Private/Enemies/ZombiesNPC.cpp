// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ZombiesNPC.h"

#include "Enemies/ZombiesRounds.h"
#include "Components/HealthComponent.h"
#include "Towers/Shelter.h"
#include "Character/SurvivorCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

//TODO: Make Zombies Anim and Sounds

// Sets default values
AZombiesNPC::AZombiesNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Velocity
	GetCharacterMovement()->MaxWalkSpeed = Velocity;

	//Health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->MaxHealth = 50.f;
	HealthComponent->Health = 50.f;
}

// Called when the game starts or when spawned
void AZombiesNPC::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AZombiesAIController>(GetController());
	SetObjToSearch();
}

// Called every frame
void AZombiesNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(AttackTimer < AttackDelay)
	{
		AttackTimer += DeltaTime;
	}
	
	SearchObj(ObjToSearch);
}


void AZombiesNPC::Move(FVector Destination)
{
	//Use pathfinding system of AIController
	if(AIController) AIController->MoveToLocation(Destination);
}

void AZombiesNPC::SearchObj(int Objective)
{
	AActor* NearestObj = nullptr;
	float NearestDistanceSq = FLT_MAX;

	if (Objective == 1) //Search Players
		{
		TArray<AActor*> FoundPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASurvivorCharacter::StaticClass(), FoundPlayers);

		for (AActor* Player : FoundPlayers)
		{
			float DistanceSq = FVector::DistSquared(GetActorLocation(), Player->GetActorLocation());
			if (DistanceSq < NearestDistanceSq)
			{
				NearestObj = Player;
				NearestDistanceSq = DistanceSq;
			}
		}
		}
	else if (Objective == 2) //Search Shelters
		{
		TArray<AActor*> FoundShelters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShelter::StaticClass(), FoundShelters);

		for (AActor* Shelter : FoundShelters)
		{
			float DistanceSq = FVector::DistSquared(GetActorLocation(), Shelter->GetActorLocation());
			if (DistanceSq < NearestDistanceSq)
			{
				NearestObj = Shelter;
				NearestDistanceSq = DistanceSq;
			}
		}
		}
	
	if (NearestObj)
	{
		if (NearestDistanceSq <= FMath::Square(AttackDistance))
		{
			Attack(NearestObj); //The enemy is near enough to attack
		}
		else
		{
			Move(NearestObj->GetActorLocation()); //Move towards the objective
		}
	}
}

void AZombiesNPC::Attack(AActor* AttackedActor)
{
	UHealthComponent* HealthComp = Cast<UHealthComponent>(AttackedActor->GetComponentByClass(UHealthComponent::StaticClass()));
	AZombiesNPC* Zombie = Cast<AZombiesNPC>(AttackedActor);
	if (HealthComp && AttackTimer>=AttackDelay && !Zombie)
	{
		HealthComp->DecrementHealth(DamageAmount);
		AttackTimer = 0.f;
	}
}

void AZombiesNPC::Spawn(FVector Location)
{
	ISpawnInterface::Spawn(Location);
	//That notify that the Zombie is occupied
	IsDied = false;
	HealthComponent->Health = HealthComponent->MaxHealth;
	
	SetActorLocation(Location);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AZombiesNPC::Die()
{
	ISpawnInterface::Die();
	
	//Update the Zombie killed for the round counter
	AZombiesRounds* Round = Cast<AZombiesRounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AZombiesRounds::StaticClass()));
	if(Round && !IsDied) Round->ZombiesKilledThisRound++;
	
	//That notify that the Zombie is ready for another use
	IsDied = true;
	
	SetActorLocation(FVector(0,0,0));
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}


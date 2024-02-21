// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ZombiesNPC.h"

#include "EngineUtils.h"
#include "Animations/ZombieAnim.h"
#include "Enemies/ZombiesRounds.h"
#include "Components/HealthComponent.h"
#include "Towers/Shelter.h"
#include "Character/SurvivorCharacter.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombiesNPC::AZombiesNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Velocity
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	
	//Health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->MaxHealth = 50;
	HealthComponent->Health = 50;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	Tags.Add(Tag);
}

// Called when the game starts or when spawned
void AZombiesNPC::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AZombiesAIController>(GetController());
	SetObjToSearch();
	
	//Animation Settings
	Animations = Cast<UZombieAnim>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AZombiesNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Animations) Animations->Animate(this);
	if(!IsDied) SearchObj(ObjToSearch);
	if(IsDied) Move(GetActorLocation()); //If is died stop moving to the last location setted
	ManageTimers(DeltaTime);
	
}

void AZombiesNPC::ManageTimers(float DeltaTime)
{
	if(AttackTimer < AttackDelay)
	{
		AttackTimer += DeltaTime;
	}
	if(IsDied)
	{
		//Wait for the animation to play and then respawn
		Animations->IsDying = true;
		DyingTimer += DeltaTime;
		if(DyingTimer >= DyingDelay)
		{
			DyingTimer = 0;
			CanSpawn = true;
		}
	}
}


void AZombiesNPC::Move(FVector Destination)
{
	//Use pathfinding system of AIController
		if(AIController) AIController->MoveToLocation(Destination);
		Animations->IsAttacking = false;

		//Let the zombie play his sound in random moments
		int Rand = FMath::RandRange(1, 10000);
		if (Rand == 5000)
		{
			AudioComponent->SetSound(GrowningSound);
			AudioComponent->Play();
		}
}

void AZombiesNPC::SearchObj(int Objective)
{
	AActor* NearestObj = nullptr;
	float NearestDistanceSq = FLT_MAX;

	if (Objective == 1) //Search Players
	{
		for (TActorIterator<ASurvivorCharacter> It(GetWorld(), ASurvivorCharacter::StaticClass()); It; ++It)
		{
			ASurvivorCharacter* Survivor = *It;
			if (Survivor && !Survivor->IsDied)
			{
				float DistanceSq = FVector::DistSquared(GetActorLocation(), Survivor->GetActorLocation());
				if (DistanceSq < NearestDistanceSq)
				{
					NearestObj = Survivor;
					NearestDistanceSq = DistanceSq;
				}
			}
		}
	}
	else if (Objective == 2) //Search Shelters
	{
		for (TActorIterator<AShelter> It(GetWorld(), AShelter::StaticClass()); It; ++It)
		{
			AShelter* Shelter = *It;
			if (Shelter)
			{
				float DistanceSq = FVector::DistSquared(GetActorLocation(), Shelter->GetActorLocation());
				if (DistanceSq < NearestDistanceSq)
				{
					NearestObj = Shelter;
					NearestDistanceSq = DistanceSq;
				}
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
			AudioComponent->SetSound(AttackSound);
			AudioComponent->Play();
			Animations->IsAttacking = true;
			HealthComp->DecrementHealth(DamageAmount);
			AttackTimer = 0.f;
		}
}

void AZombiesNPC::Spawn(FVector Location)
{
	ISpawnInterface::Spawn(Location);
	//That notify that the Zombie is occupied
	IsDied = false;
	CanSpawn = false;
	Animations->IsDying = false;
	
	HealthComponent->IncrementHealth(HealthComponent->MaxHealth);
	
	SetActorLocation(Location);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AZombiesNPC::Die()
{
	ISpawnInterface::Die();
	
	// //Update the Zombie killed for the round counter
	AZombiesRounds* Round = Cast<AZombiesRounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AZombiesRounds::StaticClass()));
	if(Round && !IsDied) Round->IncrementZombieDied();

	//That notify that the Zombie is ready for another use
	IsDied = true;
}


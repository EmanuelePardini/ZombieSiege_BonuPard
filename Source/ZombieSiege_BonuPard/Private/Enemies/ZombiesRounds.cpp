// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ZombiesRounds.h"

#include "Character/SurvivorCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombiesRounds::AZombiesRounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//The array length is the round number and the indexes values is the number of zombies for any round
	ZombiesxRounds = {25, 50, 100, 150, 200, 250};

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AZombiesRounds::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AZombiesRounds::Init, 5.0f, false);
}

// Called every frame
void AZombiesRounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageRound(DeltaTime);
}

void AZombiesRounds::SetupInitialRound()
{
	OnRoundChange.Broadcast(ActualRound);
}

void AZombiesRounds::Init()
{
	//Init is used to Jump Round 0 and reposition Zombies
	for(auto& Spawner : Spawners)
	{
		for(auto Zombie : Spawner->Zombies)
		{
			Zombie->Die(); //To begin by Round One and send Zombies to spawn location
		}
	}
}

void AZombiesRounds::ManageRound(float DeltaTime)
{
	if(!IsIntoRound)
	{ //If the Round is ended give the player a pause
		RoundTimer+=DeltaTime;
		if(RoundTimer >= RoundDelay)
		{
			IsIntoRound = true;
			RoundTimer = 0.f;
		}
	}
	else
	{//else if the player is in the round check if have to spawn zombies or terminate round
		if(ZombiesSpawnedThisRound < ZombiesxRounds[ActualRound] && ActualRound > 0) ActivateZombies();
		if(ZombiesKilledThisRound >= ZombiesxRounds[ActualRound])  TerminateRound();

		//If last Zombie play Last zombie audio
		if(ZombiesKilledThisRound+1 == ZombiesxRounds[ActualRound])
		{
			AudioComponent->SetSound(LastZombieSound);
			AudioComponent->Play();
		}
	}
}

void AZombiesRounds::IncrementZombieDied()
{
	ZombiesKilledThisRound++;
	OnZombieKilled.Broadcast(KillReward);
}

void AZombiesRounds::ActivateZombies()
{ //For any spawners spawn [Actual Round] Zombies
	for(auto& Spawner : Spawners)
	{
		for(auto Zombie : Spawner->Zombies)
		{ 
			if(Zombie)
			{
				//If the resource Zombie is free we can use that
				if(Zombie->CanSpawn)
				{
					Zombie->Spawn(Spawner->GetActorLocation());
					ZombiesSpawnedThisRound++;
				}
			}
		}
	}
}

void AZombiesRounds::TerminateRound()
{
	AudioComponent->SetSound(RoundEndSound);
	AudioComponent->Play();
	
	IsIntoRound = false;
	RoundTimer = 0.f;
	ZombiesSpawnedThisRound = 0;
	ZombiesKilledThisRound = 0;
	if(ActualRound < ZombiesxRounds.Num()-1)
	{
		OnRoundEnd.Broadcast(RoundReward);
		ActualRound++;
		OnRoundChange.Broadcast(ActualRound);
	}
	else
	{
		EndGame();
	}

	TArray<AActor*> FoundSurvivors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASurvivorCharacter::StaticClass(), FoundSurvivors);

	//Respawn/Reset Health to Survivors
	for (AActor* Actor : FoundSurvivors)
	{
		ASurvivorCharacter* Survivor = Cast<ASurvivorCharacter>(Actor);
		if (Survivor)
		{
			Survivor->Spawn(Survivor->GetActorLocation());
		}
	}
	
}

void AZombiesRounds::EndGame()
{
	UGameplayStatics::OpenLevel(this, "VictoryMap");
}




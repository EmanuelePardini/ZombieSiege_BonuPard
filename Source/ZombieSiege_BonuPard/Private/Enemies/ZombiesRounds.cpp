// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/ZombiesRounds.h"

// Sets default values
AZombiesRounds::AZombiesRounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//The array length is the round number and the indexes values is the number of zombies for any round
	ZombiesxRounds = {25, 50, 100, 150, 200, 250};
}

// Called when the game starts or when spawned
void AZombiesRounds::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void AZombiesRounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(1,1,FColor::Red,FString::Printf(TEXT("ActualRound: %d"), ActualRound));
	GEngine->AddOnScreenDebugMessage(2,1,FColor::Red,FString::Printf(TEXT("Spawned: %d"), ZombiesSpawnedThisRound));
	GEngine->AddOnScreenDebugMessage(3,1,FColor::Red,FString::Printf(TEXT("Killed: %d"), ZombiesKilledThisRound));
	ManageRound(DeltaTime);
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
	}
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
				if(Zombie->IsDied)
				{
					GEngine->AddOnScreenDebugMessage(-1,8,FColor::Red, Zombie->GetActorLabel());
					Zombie->Spawn(Spawner->GetActorLocation());
					ZombiesSpawnedThisRound++;
				}
			}
		}
	}
}

void AZombiesRounds::TerminateRound()
{
	IsIntoRound = false;
	RoundTimer = 0.f;
	ZombiesSpawnedThisRound = 0;
	ZombiesKilledThisRound = 0;
	if(ActualRound < ZombiesxRounds.Num()-1)
	{
		ActualRound++;
	}
	else
	{
		EndGame();
	}
	
}

void AZombiesRounds::EndGame()
{
	//TODO: Implement EndGame
}




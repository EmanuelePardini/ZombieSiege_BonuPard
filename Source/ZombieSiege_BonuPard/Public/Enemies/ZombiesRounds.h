// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieSpawner.h"
#include "GameFramework/Actor.h"
#include "ZombiesRounds.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API AZombiesRounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombiesRounds();

	//Spawners Management
	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<AZombieSpawner*> Spawners;

	//Round Management
	UPROPERTY(VisibleAnywhere)
	int ActualRound = 0;
	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<int> ZombiesxRounds;
	UPROPERTY(VisibleAnywhere)
	int ZombiesSpawnedThisRound = 0;
	UPROPERTY(VisibleAnywhere)
	int ZombiesKilledThisRound = 0;

	//Timer Management
	UPROPERTY(EditAnywhere, Category = "Setup")
	float SpawnTimer = 0.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float SpawnDelay = 2.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float RoundTimer = 0.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float RoundDelay = 10.f;
	UPROPERTY(VisibleAnywhere)
	bool IsIntoRound = true;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init();
	void ManageRound(float DeltaTime);
	void ActivateZombies();
	void TerminateRound();
	void EndGame();
};

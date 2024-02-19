// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieSpawner.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "ZombiesRounds.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZombieKilled, int, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEnd, int, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundChange, int, ActualRound);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	
	//Audio Management
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* RoundEndSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* LastZombieSound;

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

	UPROPERTY(BlueprintAssignable)
	FOnZombieKilled OnZombieKilled;
	UPROPERTY(BlueprintAssignable)
	FOnRoundEnd OnRoundEnd;
	UPROPERTY(BlueprintAssignable)
	FOnRoundChange OnRoundChange;
	UPROPERTY(EditAnywhere, Category = "Setup")
	int KillReward = 50;
	UPROPERTY(EditAnywhere, Category = "Setup")
	int RoundReward = 1000;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetupInitialRound();
	void Init();
	void ManageRound(float DeltaTime);
	void IncrementZombieDied();
	void ActivateZombies();
	void TerminateRound();
	void EndGame();
};

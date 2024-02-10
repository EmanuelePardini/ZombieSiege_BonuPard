// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SpawnInterface.h"
#include "ZombiesAIController.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "ZombiesNPC.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API AZombiesNPC : public ACharacter, public ISpawnInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AZombiesNPC();
	
	UPROPERTY(VisibleAnywhere)
	int ObjToSearch = 0; //0 Nothing, 1 Players, 2 Shelter
	UPROPERTY(VisibleAnywhere)
	bool IsDied = false;
	//Zombies Abilities
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AttackDistance = 150.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float DamageAmount = 10.f; 
	UPROPERTY(EditAnywhere, Category = "Setup")
	float Velocity = 250.f;

	//Attack Timer
	UPROPERTY(VisibleAnywhere, Category= "Timer")
	float AttackTimer = 0.f;
	UPROPERTY(EditAnywhere, Category= "Timer")
	float AttackDelay = 2.5f;

	//AI Controller
	UPROPERTY(EditAnywhere, Category= "Setup")
    AZombiesAIController* AIController;

	//Components
	UPROPERTY(EditAnywhere, Category = "Setup")
	UHealthComponent* HealthComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE int SetObjToSearch() {return ObjToSearch = FMath::RandRange(1,2);}
	void Move(FVector Destination);
	void SearchObj(int Objective);
	void Attack(AActor* AttackedActor);
	virtual void Spawn(FVector Location) override;
	virtual void Die() override;
};

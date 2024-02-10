// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/ZombiesNPC.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();
	
	UPROPERTY(EditAnywhere, Category = "Setup")
	TArray<AZombiesNPC*> Zombies; //Maybe 10 spawner with 5 zombies?
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

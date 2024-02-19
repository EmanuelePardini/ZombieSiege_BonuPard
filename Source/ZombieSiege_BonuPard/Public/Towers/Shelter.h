// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SpawnInterface.h"
#include "Components/HealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Shelter.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API AShelter : public AActor, public ISpawnInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShelter();
	
	UPROPERTY(EditAnywhere, Category = "Setup")
	UHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UTextRenderComponent* HealthText;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Spawn(FVector Location) override;
	virtual void Die() override;
};

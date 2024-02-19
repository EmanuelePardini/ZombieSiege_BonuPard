// Fill out your copyright notice in the Description page of Project Settings.


#include "Towers/Shelter.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AShelter::AShelter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->MaxHealth = 500.f;
	HealthComponent->Health = 500.f;

	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
}

// Called when the game starts or when spawned
void AShelter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShelter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HealthComponent) HealthText->SetText(FText::FromString(FString::FromInt(HealthComponent->Health)));
}

void AShelter::Spawn(FVector Location)
{
	ISpawnInterface::Spawn(Location);
}

void AShelter::Die()
{
	ISpawnInterface::Die();
	UGameplayStatics::OpenLevel(this, "GameOverMap");
}


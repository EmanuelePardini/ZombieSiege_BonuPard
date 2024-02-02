// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege_GameMode.h"

#include "Kismet/GameplayStatics.h"


void AZombieSiege_GameMode::BeginPlay()
{
	Super::BeginPlay();
	InitPlayers();
}

void AZombieSiege_GameMode::InitPlayers()
{
	ASurvivorController* FirstController = Cast<ASurvivorController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ASurvivorController* SecondController = Cast<ASurvivorController>(UGameplayStatics::CreatePlayer(GetWorld(), 1));
	FirstController->AssignPlayersIMC();
	SecondController->AssignPlayersIMC();
	
}

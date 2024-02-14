// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege_GameMode.h"

#include "Character/SurvivorController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


AZombieSiege_GameMode::AZombieSiege_GameMode()
{
	//Audio Settings
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = true;
	AudioComponent->VolumeMultiplier = 0.1f;
	AudioComponent->OnAudioFinished.AddDynamic(this, &AZombieSiege_GameMode::OnAudioFinished);
}

void AZombieSiege_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitPlayers();
	AudioComponent->SetSound(Soundtrack);
	AudioComponent->Play();
}

void AZombieSiege_GameMode::OnAudioFinished()
{
	AudioComponent->Play();
}

void AZombieSiege_GameMode::InitPlayers()
{
	ASurvivorController* FirstController = Cast<ASurvivorController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	FirstController->AssignPlayersIMC();
	
	if(isCoop)
	{
		ASurvivorController* SecondController = Cast<ASurvivorController>(UGameplayStatics::CreatePlayerFromPlatformUser(GetWorld(), SecondUserId, true));
    
		float DelaySeconds = 0.2f; // Delay time in seconds
		FTimerHandle UnusedHandle; // Timer handle, not used but required for the function call
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [SecondController]()
		{
			if (SecondController)
			{
				SecondController->AssignPlayersIMC();
			}
		}, DelaySeconds, false);
	}
	
}

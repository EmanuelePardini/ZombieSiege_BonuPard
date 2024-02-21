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
	AudioComponent->OnAudioFinished.AddDynamic(this, &AZombieSiege_GameMode::RestartAudio);
}

void AZombieSiege_GameMode::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle_InitPlayers;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_InitPlayers, this, &AZombieSiege_GameMode::InitPlayers, 0.2f, false);
	AudioComponent->SetSound(Soundtrack);
	AudioComponent->Play();
}

void AZombieSiege_GameMode::RestartAudio()
{
	AudioComponent->Play();
}

void AZombieSiege_GameMode::InitPlayers()
{
	FVector2D Resolution;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Resolution);
	}
                
	ASurvivorController* FirstController = Cast<ASurvivorController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	FirstController->AssignPlayersIMC();
	FirstController->SetUpHUD(FVector2D(0, 0),  FVector2D(Resolution.X, Resolution.Y / 2));
	if(IsCoop)
	{
		ASurvivorController* SecondController = Cast<ASurvivorController>(UGameplayStatics::CreatePlayer(GetWorld(), SecondUserId, true));
    
		float DelaySeconds = 0.2f; // Delay time in seconds
		FTimerHandle UnusedHandle; // Timer handle, not used but required for the function call
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [SecondController, Resolution]()
		{
			if (SecondController)
			{
				SecondController->AssignPlayersIMC();
				SecondController->SetUpHUD(FVector2D(0, Resolution.Y / 2),  FVector2D(Resolution.X, Resolution.Y / 2));
			}
		}, DelaySeconds, false);
	}
}

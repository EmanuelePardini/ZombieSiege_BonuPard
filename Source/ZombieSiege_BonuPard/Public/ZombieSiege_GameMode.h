// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundCue.h"
#include "ZombieSiege_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API AZombieSiege_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AZombieSiege_GameMode();
	
	UPROPERTY(EditAnywhere)
	bool IsCoop = true; //To switch by single and multiplayer
	UPROPERTY(EditAnywhere)
	FPlatformUserId SecondUserId = 1;//On my x360 controller emulator i used the second slot, if needed change that
	//Audio Management
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* Soundtrack;

	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void RestartAudio();
	virtual void InitPlayers(); 
};

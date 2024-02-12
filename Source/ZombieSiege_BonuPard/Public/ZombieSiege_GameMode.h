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
	
//TODO: Begin HUD
	UPROPERTY(EditAnywhere)
	bool isCoop = true; //To switch by single and multiplayer
	UPROPERTY(EditAnywhere)
	FPlatformUserId SecondUserId = 1; //On my x360 controller emulator i used the second slot, if needed change that

	
protected:
	virtual void BeginPlay() override;
	virtual void InitPlayers(); 
};

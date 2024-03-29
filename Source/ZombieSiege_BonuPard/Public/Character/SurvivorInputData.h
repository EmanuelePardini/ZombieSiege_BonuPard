// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SurvivorInputData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API USurvivorInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	FString MappingName;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	class UInputAction* Move;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Look;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Jump;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Run;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Crouch;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Interact;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* ManageCoop;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Aim;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Shoot;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Reload;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* ToggleBuild;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Build;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* SwapBuildable;
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	UInputAction* Revive;
};

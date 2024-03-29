// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomHUD.h"
#include "SurvivorCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "SurvivorController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API ASurvivorController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	class UInputMappingContext* IMC;

	//For HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HUD")
	ACustomHUD* PlayerHUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<AHUD> HUDClass;

protected:
	//For Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	class USurvivorInputData* InputData;


	//For Survivor Control
	UPROPERTY(VisibleAnywhere)
	ASurvivorCharacter* SurvivorCharacter;
	
	//To Manage Actions Timer and avoid Spam
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timer")
	bool CanManageCoop;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Timer")
	float ManageCoopTimer = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Timer")
	float ManageCoopDelay = 10;

	virtual void BeginPlay() override;
	
public:
	void SetUpHUD(FVector2D Position, FVector2D Size);
	void AssignPlayersIMC();
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	//For Timers Management
	void ManageTimers(float DeltaSeconds);
	
	//Primary Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Jump Manage
	void Jump(const FInputActionValue& Value);

	//Run Manage
	void Run(const FInputActionValue& Value);
	void EndRun(const FInputActionValue& Value);

	//Crouch Manage
	void Crouch();
	void UnCrouch();

	//Combat manage
	void Aim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StopShoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);

	//Build Manage
	void ToggleBuild(const FInputActionValue& Value);
	void Build(const FInputActionValue& Value);
	void SwapBuildable(const FInputActionValue& InputActionValue);

	//Interaction Manage
	void Interact(const FInputActionValue& Value);
	void Revive(const FInputActionValue& Value);

	//PlayerActivation
	void ManageCoop(const FInputActionValue& Value);
};

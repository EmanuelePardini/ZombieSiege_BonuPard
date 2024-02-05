// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Animations/SurvivorAnim.h"
#include "SurvivorCharacter.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API ASurvivorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivorCharacter();

	//Components Declaration

	//Animations Declaration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	USurvivorAnim* Animations;
	//Movement Parameters
	UPROPERTY(VisibleAnywhere)
	bool IsCrouched = false;
	UPROPERTY(VisibleAnywhere)
	bool IsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsRunning = false;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Primary Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Jump Manage
	void DoJump(const FInputActionValue& Value);
	
	//Run Manage
	void Run(const FInputActionValue& Value);
	void EndRun(const FInputActionValue& Value);

	//Crouch Manage
	void Crouch(const FInputActionValue& Value);
	void UnCrouch(const FInputActionValue& Value);
	
	//Interaction Manage
	void Interact(const FInputActionValue& Value);
	
	//Drop Items Manage
	void Drop(const FInputActionValue& Value);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

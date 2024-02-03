// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EngineUtils.h"
#include "SurvivorInputData.h"
#include "Kismet/GameplayStatics.h"

void ASurvivorController::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivorController::AssignPlayersIMC()
{
	//Method used by Gamemode to assign players input mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
			Subsystem->AddMappingContext(IMC, 0);
			SurvivorCharacter = Cast<ASurvivorCharacter>(GetPawn());
	}
}

void ASurvivorController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ManageTimers(DeltaSeconds);
}

void ASurvivorController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(InputData->Move, ETriggerEvent::Triggered, this, &ASurvivorController::Move);

		//Looking
		EnhancedInputComponent->BindAction(InputData->Look, ETriggerEvent::Triggered, this, &ASurvivorController::Look);
		
		//Jumping
		EnhancedInputComponent->BindAction(InputData->Jump, ETriggerEvent::Triggered, this, &ASurvivorController::Jump);
		
		//Interacting
		EnhancedInputComponent->BindAction(InputData->Interact, ETriggerEvent::Triggered, this, &ASurvivorController::Interact);
		EnhancedInputComponent->BindAction(InputData->Drop, ETriggerEvent::Triggered, this, &ASurvivorController::Drop);

		//Crouching
		EnhancedInputComponent->BindAction(InputData->Crouch, ETriggerEvent::Triggered, this, &ASurvivorController::Crouch);
		EnhancedInputComponent->BindAction(InputData->Crouch, ETriggerEvent::Completed, this, &ASurvivorController::UnCrouch);

		//Running
		EnhancedInputComponent->BindAction(InputData->Run, ETriggerEvent::Triggered, this, &ASurvivorController::Run);
		EnhancedInputComponent->BindAction(InputData->Run, ETriggerEvent::Completed, this, &ASurvivorController::EndRun);
	}
}

void ASurvivorController::ManageTimers(float DeltaSeconds)
{
	InteractTimer += DeltaSeconds;
	
	if(InteractTimer >= InteractDelay)
	{
		CanInteract = true;
		InteractTimer = 0;
	}
}

void ASurvivorController::Move(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Move(Value);
	}
}

void ASurvivorController::Look(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Look(Value);
	}
}

void ASurvivorController::Jump(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Jump();
	}
}

void ASurvivorController::Run(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Run(Value);
	}
}

void ASurvivorController::EndRun(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->EndRun(Value);
	}
}

void ASurvivorController::Crouch(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Crouch();
	}
}

void ASurvivorController::UnCrouch(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->UnCrouch();
	}
}

void ASurvivorController::Interact(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Interact(Value);
	}
}

void ASurvivorController::Drop(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Drop(Value);
	}
}

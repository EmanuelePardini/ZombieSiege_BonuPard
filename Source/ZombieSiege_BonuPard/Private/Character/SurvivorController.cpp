// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SurvivorController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/SurvivorInputData.h"

void ASurvivorController::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivorController::SetUpHUD(FVector2D Position, FVector2D Size)
{
	if (HUDClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		PlayerHUD = GetWorld()->SpawnActor<ACustomHUD>(HUDClass, SpawnParams);
	}
	
	if (IsLocalPlayerController() && PlayerHUD)
	{
		PlayerHUD->InitTriggerReference();
		PlayerHUD->MainUserWidget->SetPositionInViewport(Position);
		PlayerHUD->MainUserWidget->SetDesiredSizeInViewport(Size);
		PlayerHUD->MainUserWidget->AddToViewport();
	}
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
		EnhancedInputComponent->BindAction(InputData->Revive, ETriggerEvent::Triggered, this, &ASurvivorController::Revive);
		EnhancedInputComponent->BindAction(InputData->Interact, ETriggerEvent::Triggered, this, &ASurvivorController::Interact);

		//Coop
		EnhancedInputComponent->BindAction(InputData->ManageCoop, ETriggerEvent::Triggered, this, &ASurvivorController::ManageCoop);

		//Crouching
		EnhancedInputComponent->BindAction(InputData->Crouch, ETriggerEvent::Triggered, this, &ASurvivorController::Crouch);
		EnhancedInputComponent->BindAction(InputData->Crouch, ETriggerEvent::Completed, this, &ASurvivorController::UnCrouch);

		//Aiming
		EnhancedInputComponent->BindAction(InputData->Aim, ETriggerEvent::Triggered, this, &ASurvivorController::Aim);
		EnhancedInputComponent->BindAction(InputData->Aim, ETriggerEvent::Completed, this, &ASurvivorController::StopAim);

		//Shooting
		EnhancedInputComponent->BindAction(InputData->Shoot, ETriggerEvent::Triggered, this, &ASurvivorController::Shoot);
		EnhancedInputComponent->BindAction(InputData->Shoot, ETriggerEvent::Completed, this, &ASurvivorController::StopShoot);

		//Reloading
		EnhancedInputComponent->BindAction(InputData->Reload, ETriggerEvent::Triggered, this, &ASurvivorController::Reload);

		//Building
		EnhancedInputComponent->BindAction(InputData->ToggleBuild, ETriggerEvent::Started, this, &ASurvivorController::ToggleBuild);
		EnhancedInputComponent->BindAction(InputData->Build, ETriggerEvent::Started, this, &ASurvivorController::Build);
		EnhancedInputComponent->BindAction(InputData->SwapBuildable, ETriggerEvent::Triggered, this, &ASurvivorController::SwapBuildable);
		
		//Running
		EnhancedInputComponent->BindAction(InputData->Run, ETriggerEvent::Triggered, this, &ASurvivorController::Run);
		EnhancedInputComponent->BindAction(InputData->Run, ETriggerEvent::Completed, this, &ASurvivorController::EndRun);
	}
}

void ASurvivorController::ManageTimers(float DeltaSeconds)
{
	ManageCoopTimer += DeltaSeconds;
	
	if(ManageCoopTimer >= ManageCoopDelay)
	{
		CanManageCoop = true;
		ManageCoopTimer = 0;
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
		SurvivorCharacter->DoJump(Value);
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

void ASurvivorController::Crouch()
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->DoCrouch();
	}
}

void ASurvivorController::UnCrouch()
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->DoUnCrouch();
	}
}

void ASurvivorController::Aim(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Aim(Value);
	}
}

void ASurvivorController::StopAim(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->StopAim(Value);
	}
}

void ASurvivorController::Shoot(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Shoot(Value);
	}
}

void ASurvivorController::StopShoot(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->StopShoot(Value);
	}
}

void ASurvivorController::Reload(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Reload(Value);
	}
}

void ASurvivorController::ToggleBuild(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->ToggleBuild(Value);
	}
}

void ASurvivorController::Build(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Build(Value);
	}
}

void ASurvivorController::SwapBuildable(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->SwapBuildable(Value);
	}
}

void ASurvivorController::Interact(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Interactor(Value);
	}
}

void ASurvivorController::Revive(const FInputActionValue& Value)
{
	if(SurvivorCharacter)
	{
		SurvivorCharacter->Revive(Value);
	}
}

void ASurvivorController::ManageCoop(const FInputActionValue& Value)
{
	if(SurvivorCharacter && CanManageCoop)
	{
		SurvivorCharacter->ManageCoop(Value);
		CanManageCoop = false;
	}
}

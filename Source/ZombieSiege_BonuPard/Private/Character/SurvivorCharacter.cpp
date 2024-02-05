// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege_BonuPard/Public/Character/SurvivorCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASurvivorCharacter::ASurvivorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Movement Settings
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

// Called when the game starts or when spawned
void ASurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Animation Settings
	Animations = Cast<USurvivorAnim>(GetMesh()->GetAnimInstance());
}

void ASurvivorCharacter::Move(const FInputActionValue& Value)
{
	// Obtains the controller's rotation and create a rotation on the yaw axis
	FVector2d MovementValue = Value.Get<FVector2d>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Gets the forward and right directions based on the Yaw rotation
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Adds movement input along the forward axis (ForwardDirection) and the right axis (RightDirection)
	AddMovementInput(ForwardDirection, MovementValue.Y);
	AddMovementInput(RightDirection, MovementValue.X);
}

void ASurvivorCharacter::Look(const FInputActionValue& Value)
{
	FVector2d LookValue = Value.Get<FVector2d>();
	
	// Adds input to control the rotation
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void ASurvivorCharacter::DoJump(const FInputActionValue& Value)
{
	if(!IsCrouched)
	{
		Jump();
	}
}

void ASurvivorCharacter::Run(const FInputActionValue& Value)
{
	if(!IsCrouched && !IsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		Animations->IsRunning = true;
		IsRunning = true;
	}
}

void ASurvivorCharacter::EndRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	Animations->IsRunning = false;
	IsRunning = false;
}

void ASurvivorCharacter::Crouch(const FInputActionValue& Value)
{
	if(!IsRunning)
	{
		IsCrouched = true;
		Animations->IsCrouched = true;
	}
}

void ASurvivorCharacter::UnCrouch(const FInputActionValue& Value)
{
	IsCrouched = false;
	Animations->IsCrouched = false;
}

void ASurvivorCharacter::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString(TEXT("To implement Interact")));
}

void ASurvivorCharacter::Drop(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString(TEXT("To implement Drop")));
}

// Called every frame
void ASurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Animations)
	{
		Animations->Animate(this);
	}
}

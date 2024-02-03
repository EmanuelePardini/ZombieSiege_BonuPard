// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorAnim.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USurvivorAnim::Animate(ACharacter* Character)
{
		//Get absolute values of Velocity
		FVector Velocity = Character->GetCharacterMovement()->Velocity.GetAbs();
	
		//Update the parameters based on the character movement
		Grounded = !Character->GetCharacterMovement()->IsFalling();
		SpeedX = Velocity.X;
		SpeedY = Velocity.Y;
}


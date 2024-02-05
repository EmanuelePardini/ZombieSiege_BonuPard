// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/SurvivorAnim.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void USurvivorAnim::Animate(ACharacter* Character)
{
	//Get absolute values of Velocity
	FVector Velocity = Character->GetCharacterMovement()->Velocity.GetAbs();
	//Update the parameters based on the character movement
	IsGrounded = !Character->GetCharacterMovement()->IsFalling();
	
	/*IsCrouched, IsRunning, IsAiming
	are setted in the Character because the built-in function of UE don't work properly*/
	
	SpeedX = Velocity.X;
	SpeedY = Velocity.Y;
	Velocity = Velocity.GetSafeNormal();
	Direction = FMath::RadiansToDegrees(FMath::Atan2(SpeedY, SpeedX));
}

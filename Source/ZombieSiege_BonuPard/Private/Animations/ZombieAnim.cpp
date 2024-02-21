// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ZombieAnim.h"

void UZombieAnim::Animate(AZombiesNPC* Character)
{
	Speed = Character->Speed;
	if(Speed > 0 && Speed < 300)
	{
		IsWalking = true;
		IsRunning = false;
	}
	else if(Speed >= 300){
		IsRunning = true;
		IsWalking = false;
	}
	else
	{
		IsRunning = false;
		IsWalking = false;
	}
}

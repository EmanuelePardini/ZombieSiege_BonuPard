// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemies/ZombiesNPC.h"
#include "ZombieAnim.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API UZombieAnim : public UAnimInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsWalking = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsDying = false;
	
	void Animate(AZombiesNPC* Character);
	FVector GetDeltaRoot(){return ConsumeExtractedRootMotion(1).GetRootMotionTransform().GetLocation();}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SurvivorAnim.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API USurvivorAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	//All public variables are setted in the Character Input settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsCrouched = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsShooting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsReloading = false;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpeedX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpeedY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsGrounded = true;



public:
	void Animate(ACharacter* Character);
	FVector GetDeltaRoot(){return ConsumeExtractedRootMotion(1).GetRootMotionTransform().GetLocation();}
};

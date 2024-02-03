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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpeedX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpeedY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Grounded = true;

public:
	void Animate(ACharacter* Character);
	FVector GetDeltaRoot(){return ConsumeExtractedRootMotion(1).GetRootMotionTransform().GetLocation();}
};

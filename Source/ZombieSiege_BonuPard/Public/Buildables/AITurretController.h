// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Turret.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Sound/SoundCue.h"
#include "AITurretController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESIEGE_BONUPARD_API AAITurretController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentTargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	bool ToPlayAudio = true;
	
	AAITurretController();

	void SetupData(const FAIDataForSightConfig* AISightConfigData);
	
	void UpdateTurretRotation(float DeltaTime);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfig;


	


	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(EditDefaultsOnly, Category = "Perception")
	TSubclassOf<UAISense> SenseToUse;
};

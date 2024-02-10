// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/SpawnInterface.h"
#include "Animations/SurvivorAnim.h"
#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "Sound/SoundCue.h"
#include "SurvivorCharacter.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API ASurvivorCharacter : public ACharacter, public ISpawnInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASurvivorCharacter();

	//Components Declaration
	UPROPERTY(EditAnywhere, Category = "Setup")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Setup")
	UProjectileComponent* ProjectileComponent;
	
	//Animations Declaration
	UPROPERTY()
	USurvivorAnim* Animations;
	
	//Movement Parameters
	UPROPERTY(VisibleAnywhere)
	bool IsCrouched = false;
	UPROPERTY(VisibleAnywhere)
	bool IsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsShooting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsReloading = false;

	//Timers
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadDelay = 2.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadTimer = 0.f;

	//Audio Management
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundCue* ShootingSound;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Primary Actions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//Jump Manage
	void DoJump(const FInputActionValue& Value);
	
	//Run Manage
	void Run(const FInputActionValue& Value);
	void EndRun(const FInputActionValue& Value);

	//Crouch Manage
	void Crouch(const FInputActionValue& Value);
	void UnCrouch(const FInputActionValue& Value);

	//Combat manage
	void Aim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StopShoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void ManageReload(float DeltaTime); //Once the action started doesn't need the player to keep the button
	
	//Interaction Manage
	void Interact(const FInputActionValue& Value);
	
	//Drop Items Manage
	void Drop(const FInputActionValue& Value);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Spawn(FVector Location) override;
	virtual void Die() override;
};

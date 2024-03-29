// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BuildSystem.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/InventoryComponent.h"
#include "Components/LineTraceComponent.h"
#include "Interfaces/SpawnInterface.h"
#include "Animations/SurvivorAnim.h"
#include "Components/HealthComponent.h"
#include "Components/MoneySystemComponent.h"
#include "Components/ProjectileComponent.h"
#include "Sound/SoundCue.h"
#include "SurvivorCharacter.generated.h"

UCLASS()
class ZOMBIESIEGE_BONUPARD_API ASurvivorCharacter : public ACharacter, public ISpawnInterface, public IInteractableInterface
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
	UPROPERTY(EditAnywhere, Category = "Setup")
	ULineTraceComponent* LineTraceComponent;
	UPROPERTY(EditAnywhere, Category = "Setup")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, Category="Setup")
	UBuildSystem* BuildSystemComponent;
	UPROPERTY(EditAnywhere, Category = "Setup")
	UMoneySystemComponent* MoneySystemComponent;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsDied = false;

	
	//Timers
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadDelay = 2.f;
	UPROPERTY(EditAnywhere, Category = "Setup")
	float ReloadTimer = 0.f;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle IsRevivingTimer;
	UPROPERTY(VisibleAnywhere)
	int ReviveProgression = 0;
	UPROPERTY(VisibleAnywhere)
	int ReviveTotal = 50;

	

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
	void DoCrouch();
	void DoUnCrouch();

	//Combat manage
	void Aim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StopShoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void ManageReload(float DeltaTime); //Once the action started doesn't need the player to keep the button

	//Build Manage
	void ToggleBuild(const FInputActionValue& Value);
	void Build(const FInputActionValue& Value);
	void SwapBuildable(const FInputActionValue& InputActionValue);
	void Revive(const FInputActionValue& Value);

	//Interaction Manage
	void Interactor(const FInputActionValue& Value);
	virtual FText Interact(AActor* Interactor) override;
	void DecreaseRevive();
	
	//Player Activation Manage
	void ManageCoop(const FInputActionValue& Value);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Spawn(FVector Location) override;
	virtual void Die() override;
	bool IsAnyOneAlive();
};

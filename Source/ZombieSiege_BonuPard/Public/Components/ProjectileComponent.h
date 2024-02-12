// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESIEGE_BONUPARD_API UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileComponent();
	//Weapon Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	float ProjectileDistance = 10000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setup")
	FName InteractionMask = "OverlapAllDynamic";
	UPROPERTY(EditAnywhere, Category ="Setup")
	float DamageAmount = 5;

	//Attachment Variables
	UPROPERTY()
	UStaticMeshComponent* WeaponComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Setup")
	FString WeaponComponentName = "Rifle";

	//Loader Variables
	UPROPERTY(EditAnywhere, Category = "Setup")
	int LoaderAmmo = 50;
	UPROPERTY()
	int ActualAmmo = LoaderAmmo;
	//TODO: Ammo Packs and total ammo?
	// UPROPERTY(EditAnywhere, Category = "Setup")
	// int TotalAmmo = LoaderAmmo * 10;
	UPROPERTY(EditAnywhere, Category ="Setup")
	float ShootDelay = 0.1f;
	UPROPERTY(VisibleAnywhere)
	float ShootTimer = 0.f;
	UPROPERTY(VisibleAnywhere)
	bool CanShoot = false;
	
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ManageShootTimer(float DeltaTime);
	void Shoot();
	void AttachProjectileLine();
};

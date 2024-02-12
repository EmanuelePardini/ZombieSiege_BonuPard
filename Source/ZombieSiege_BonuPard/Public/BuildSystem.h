// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Turret.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "BuildSystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESIEGE_BONUPARD_API UBuildSystem : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Setup")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category="Setup")
	class ASurvivorCharacter* PlayerReference;

	UPROPERTY(VisibleAnywhere, Category="Setup")
	float BuildDistance = 400.f;

	UPROPERTY(EditAnywhere, Category = "Build System")
	float MinimumDistance = 250.0f;

	UPROPERTY(VisibleAnywhere, Category="Build System")
	bool bIsBuilding = false;

	UPROPERTY(EditAnywhere, Category="Build System")
	TArray<TSubclassOf<ATurret>> Buildables;

	UPROPERTY(EditAnywhere, Category="Build System")
	UInventoryComponent* CharacterInventory;

	UPROPERTY(EditAnywhere, Category = "Build System")
	UDataTable* ItemDataTable; 

	UPROPERTY(VisibleAnywhere, Category="Build System")
	ATurret* CurrentItem;

	UPROPERTY(VisibleAnywhere, Category="Build System")
	FTimerHandle BuildDelayHandle;

	UPROPERTY(VisibleAnywhere, Category="Build System")
	int BuildableIdx = 0;

public:	
	// Sets default values for this component's properties
	UBuildSystem();

	void ToggleBuildMode();
	void Build();
	void SwapBuildable(float AxisValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	double GetGroundHitLocation() const;
	FVector GetBuildLocation() const;
	FRotator GetBuildRotation() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Buildable.h"
#include "Items/ItemData.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

USTRUCT(BlueprintType)
struct FAIDataForSightConfig
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	mutable float AISightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	mutable float AILoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	mutable float AISightAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	mutable float AIFieldOfView;
};

UENUM(BlueprintType)
enum class ETurretLevel : uint8
{
	Level1,
	Level2,
	Level3
};


UCLASS()
class ZOMBIESIEGE_BONUPARD_API ATurret : public APawn, public IBuildable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Buildable")
	class UBoxComponent* CollisionBox;

public:	
	ATurret();

	UPROPERTY(EditAnywhere, Category="Buildable")
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditAnywhere, Category="Buildable")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category="Buildable")
	class UArrowComponent* ProjectileStartPoint;

	UPROPERTY(EditAnywhere, Category="Buildable")
	UStaticMeshComponent* InvalidMesh;

	UPROPERTY(EditAnywhere, Category="Buildable")
	FAIDataForSightConfig AIData;

	UPROPERTY(EditAnywhere, Category="Turret")
	ETurretLevel TurretLevel;

	/**
	 * @brief Reference to the data table used for item information.
	 * @details This property allows selecting a row from the specified data table to define the item's properties.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UDataTable> DataTableReference;

	/**
	 * @brief Name of the row in the data table representing this item.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName RowName;
	
	/**
	 * @brief Cached item data
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FItemData ItemData;

	/**
	 * @brief The quantity of item to add to his inventory on BeginPlay (useful for bootstrapping)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Quantity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Sell();
	void Upgrade();

	void Build();
	const FAIDataForSightConfig* GetAIData() const;
};

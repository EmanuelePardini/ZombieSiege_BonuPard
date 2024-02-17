#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Data")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Data")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Data")
	float Price;
};

UENUM(BlueprintType)
enum class ETurretLevel : uint8
{
	Level1,
	Level2,
	Level3
};

USTRUCT(BlueprintType)
struct ZOMBIESIEGE_BONUPARD_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<AActor> ActorReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UStaticMesh> PreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Levels")
	TMap<ETurretLevel, FItemLevelData> Levels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AISightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AISightAge;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AILoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIFieldOfView;
};
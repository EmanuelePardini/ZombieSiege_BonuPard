#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "BuyableItem.generated.h"
/**
 * @class ABuyableItem
 * @brief Serves as a base class for interactable items.
 */
UCLASS(Blueprintable)
class ZOMBIESIEGE_BONUPARD_API ABuyableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuyableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

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
	int32 Quantity = 100;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Buy(AActor* Buyer);

	/**
	 * @brief Called when RowName or DataTableReference is changed to automatically update the item data.
	 * @param PropertyChangedEvent 
	 */
	void LoadRow(FPropertyChangedEvent& PropertyChangedEvent);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};


#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "InventorySlot.generated.h"

/**
 *  @class UInventorySlot
 *  @brief Represents a slot in an inventory.
 *  @description This class is used to keep track of the items in an inventory on a per-slot basis.
 */
UCLASS(BlueprintType)
class ZOMBIESIEGE_BONUPARD_API UInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FItemData ItemData;

	UPROPERTY(VisibleAnywhere)
	int32 Quantity;

	/**
	 * @brief Initializes the slot with the specified item data and quantity.
	 * @param InItemData Item data to initialize the slot with
	 * @param InitialQuantity Quantity to initialize the slot with
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void Init(const FItemData& InItemData, int32 InitialQuantity);

	UFUNCTION(BlueprintPure, Category = "Inventory Slot")
	const FItemData& GetItemData() const { return ItemData; }

	UFUNCTION(BlueprintPure, Category = "Inventory Slot")
	FORCEINLINE int32 GetQuantity() const { return Quantity; }
	

};

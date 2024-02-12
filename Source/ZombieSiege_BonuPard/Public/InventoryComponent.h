#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "ItemData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


/**
 * @class UInventoryComponent
 * @brief Manages the inventory of an Actor.
 * 
 * This component allows an actor to have an inventory system where items can be added, removed, and dropped.
 */
UCLASS( ClassGroup=("Inventory"), meta=(BlueprintSpawnableComponent) )
class ZOMBIESIEGE_BONUPARD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	/**
	 * @brief Data table used for storing all the items.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UDataTable* ItemDataTable;

protected:
	virtual void BeginPlay() override;

	/**
	 * @brief An array of inventory slots representing the items in the inventory.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UInventorySlot*> InventoryArray;
	
	/**
	* @brief A map representing the inventory, where keys are item IDs and values are corresponding inventory slots.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, UInventorySlot*> Inventory;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Adds an item to the inventory
	 * @param ItemID The ID of the item to add
	 * @param Quantity The quantity to add
	 */
	void AddItem(const FName& ItemID, uint8 Quantity = 1);

	/**
	* @brief Remove items from the inventory.
	* @param ItemID The ID of the item to remove.
	* @param Quantity The quantity of the item to remove.
	 */
	void RemoveItem(const FName& ItemID, uint8 Quantity);

	/**
	 * @brief Give items to another inventory.
	 * @param OtherInventory The inventory to give items to.
	 * @param ItemID The ID of the item to give.
	 * @param Quantity The quantity of the item to give.
	 */
	void GiveItemsTo(UInventoryComponent* OtherInventory, const FName& ItemID, uint8 Quantity);

	
	/**
	 * @brief Spawns a random item in front of the owner of this component.
	 */
	void DropRandomItem();

	FORCEINLINE TArray<UInventorySlot*> GetInventoryArray() const { return InventoryArray; }
	FORCEINLINE TMap<FName, UInventorySlot*> GetInventory() const { return Inventory; }

private:
	/**
	 * @brief Create an inventory slot for an item.
	 * @param ItemData The Item Data of the item from the ItemDataTable.
	 * @param Quantity The quantity of the item.
	 */
	void CreateInventorySlot(const FItemData& ItemData, int32 Quantity);
};

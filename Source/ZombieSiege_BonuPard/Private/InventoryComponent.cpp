

#include "InventoryComponent.h"

#include "..\Public\Turret.h"
#include "ZombieSiege_BonuPard//Public/ItemData.h"

UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddItem(const FName& ItemID, const uint8 Quantity)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, FString(""));
	if (!ItemData) return;

	if (Inventory.Contains(ItemID))
	{
		if (ItemData->bIsStackable)
		{
			Inventory[ItemID]->Quantity += Quantity;
		}
	}
	else
	{
		if (ItemData->bIsStackable) {
			CreateInventorySlot(*ItemData, Quantity);
		}
		else
		{
			for (int i = 0; i < Quantity; i++)
			{
				CreateInventorySlot(*ItemData, 1);
			}
		}
	}
}

void UInventoryComponent::RemoveItem(const FName& ItemID, uint8 Quantity)
{
	const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, FString(""));
	
	if (!ItemData) return;
	
	if (Inventory.Contains(ItemID) && ItemData->bIsStackable)
	{
		if (Inventory[ItemID]->Quantity > Quantity)
		{
			Inventory[ItemID]->Quantity -= Quantity;
		}
		else
		{
			InventoryArray.Remove(Inventory[ItemID]);
			Inventory.Remove(ItemID);
		}
	} else if (!ItemData->bIsStackable)
	{
		for (TPair<FName, UInventorySlot*>& Slot : Inventory)
		{
			if (Slot.Key.ToString().StartsWith(ItemID.ToString()))
			{
				InventoryArray.Remove(Slot.Value);
				Inventory.Remove(Slot.Key);
			}
		}
	}
}

void UInventoryComponent::GiveItemsTo(UInventoryComponent* OtherInventory, const FName& ItemID, uint8 Quantity)
{
	OtherInventory->AddItem(ItemID, Quantity);
	RemoveItem(ItemID, Quantity);
}

void UInventoryComponent::CreateInventorySlot(const FItemData& ItemData, const int32 Quantity)
{
	if (UInventorySlot* NewSlot = NewObject<UInventorySlot>())
	{
		NewSlot->Init(ItemData, Quantity);
		InventoryArray.Add(NewSlot);

		FString UniqueID = ItemData.ItemID.ToString();
		
		if (ItemData.bIsStackable)
		{
			Inventory.Add(ItemData.ItemID, NewSlot);
		}
		else
		{
			int8 i = 0;
			while (Inventory.Contains(*UniqueID))
			{
				UniqueID = FString::Printf(TEXT("%s_%d"), *ItemData.ItemID.ToString(), i);
				i++;
			}

			Inventory.Add(*UniqueID, NewSlot);
		}
	}
}

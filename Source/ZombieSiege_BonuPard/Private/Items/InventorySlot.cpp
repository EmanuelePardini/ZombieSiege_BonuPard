// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InventorySlot.h"

void UInventorySlot::Init(const FItemData& InItemData, int32 InitialQuantity)
{
	Quantity = InitialQuantity;
	ItemData = InItemData;
}

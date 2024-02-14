// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BuyableItem.h"
#include "Components/InventoryComponent.h"

// Sets default values
ABuyableItem::ABuyableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void ABuyableItem::BeginPlay()
{
	Super::BeginPlay();

	if (DataTableReference)
	{
		FItemData* LoadedItemData = DataTableReference->FindRow<FItemData>(RowName, FString());
		if (LoadedItemData)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Item %s loaded"), *RowName.ToString()));
			GetComponentByClass<UInventoryComponent>()->AddItem(LoadedItemData->ItemID, Quantity);
		}
	}
}

// Called every frame
void ABuyableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuyableItem::Buy(AActor* Buyer)
{
	// TODO: Implement cost and currency
	GetComponentByClass<UInventoryComponent>()->GiveItemsTo(Buyer->GetComponentByClass<UInventoryComponent>(), RowName, 1);
}

void ABuyableItem::LoadRow(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ABuyableItem, DataTableReference) ||
		ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ABuyableItem, RowName))
	{
		if (DataTableReference)
		{
			FItemData* LoadedItemData = DataTableReference->FindRow<FItemData>(RowName, FString());
			if (LoadedItemData)
			{
				ItemData = *LoadedItemData;
				if (ItemData.PreviewMesh)
				{
					Mesh->SetStaticMesh(ItemData.PreviewMesh.LoadSynchronous());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("PreviewMesh not set for item %s"), *RowName.ToString());
				}
			}
		}
	}
}

void ABuyableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadRow(PropertyChangedEvent);
}

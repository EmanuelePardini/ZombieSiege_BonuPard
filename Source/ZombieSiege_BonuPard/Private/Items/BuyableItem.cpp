// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BuyableItem.h"
#include "Components/InventoryComponent.h"
#include "Components/MoneySystemComponent.h"

// Sets default values
ABuyableItem::ABuyableItem()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuyableItem::BeginPlay()
{
    Super::BeginPlay();

    if (DataTableReference)
    {
        FItemData* LoadedItemData = DataTableReference->FindRow<FItemData>(RowName, FString());
        if (LoadedItemData) GetComponentByClass<UInventoryComponent>()->AddItem(LoadedItemData->ItemID, Quantity);
        ItemData = *LoadedItemData;
    }
}

// Called every frame
void ABuyableItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

FText ABuyableItem::Interact(AActor* Interactor)
{
    UMoneySystemComponent* MoneySystem = Interactor->GetComponentByClass<UMoneySystemComponent>();
    if (!MoneySystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("No MoneySystemComponent found on %s"), *Interactor->GetName());
        return FText::FromString("No MoneySystemComponent found");
    }
    if (MoneySystem->CanAfford(ItemData.Price))
    {
        MoneySystem->SpendMoney(ItemData.Price);
        GetComponentByClass<UInventoryComponent>()->GiveItemsTo(Interactor->GetComponentByClass<UInventoryComponent>(), RowName, 1);
        return FText::FromString("Bought " + RowName.ToString());
    }
    else
    {
        return FText::FromString("Not enough money");
    }
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

// void ABuyableItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
//     Super::PostEditChangeProperty(PropertyChangedEvent);
//     LoadRow(PropertyChangedEvent);
// }
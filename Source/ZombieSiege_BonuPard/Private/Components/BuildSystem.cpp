// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BuildSystem.h"

#include "Character/SurvivorCharacter.h"

// Sets default values for this component's properties
UBuildSystem::UBuildSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBuildSystem::ToggleBuildMode()
{
    bIsBuilding = !bIsBuilding;
}

void UBuildSystem::Build()
{
    FHitResult HitResult;
    FVector BuildLocation = GetBuildLocation();
    if (CharacterInventory->GetInventoryArray().Num() == 0) return;
    FName ItemData = CharacterInventory->GetInventoryArray()[BuildableIdx]->GetItemData().ItemID;

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        BuildLocation,
        BuildLocation,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeBox(FVector(1, 1, 1))
    );
    
    if (CurrentItem != nullptr && bIsBuilding && !bHit)
    {
        CurrentItem->Build();
        CurrentItem = nullptr;
        CharacterInventory->RemoveItem(ItemData, 1);
    }
}

void UBuildSystem::SwapBuildable(float AxisValue)
{
    if (!bIsBuilding) return;

    const TArray<UInventorySlot*> InventoryArray = CharacterInventory->GetInventoryArray();

    if (InventoryArray.Num() == 0) return;
    if (AxisValue > 0)
    {
        BuildableIdx -= 1;
    }
    else if (AxisValue < 0)
    {
        BuildableIdx += 1;
    }
    
    if (BuildableIdx < 0) BuildableIdx = InventoryArray.Num() - 1;
    if (BuildableIdx >= InventoryArray.Num()) BuildableIdx = 0;
    
    if (CurrentItem != nullptr)
    {
        CurrentItem->Destroy();
        CurrentItem = nullptr;
    }
}

// Called when the game starts
void UBuildSystem::BeginPlay()
{
    Super::BeginPlay();

    PlayerReference = Cast<ASurvivorCharacter>(GetOwner());
    if (PlayerReference)
    {
        Camera = PlayerReference->GetComponentByClass<UCameraComponent>();
        CharacterInventory = PlayerReference->FindComponentByClass<UInventoryComponent>();
        ItemDataTable = CharacterInventory->ItemDataTable;
    }
}

double UBuildSystem::GetGroundHitLocation() const
{
    FVector CameraLocation = Camera->GetComponentLocation();
    FVector Start = CameraLocation;
    FVector End = CameraLocation - FVector(0, 0, BuildDistance);  // Adjust the trace length as needed

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerReference);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    if (bHit)
    {
        return HitResult.Location.Z;
    }

    return 0;
}

FVector UBuildSystem::GetBuildLocation() const
{
    FVector Direction = Camera->GetForwardVector() * BuildDistance;
    FVector BuildLocation = Direction + PlayerReference->GetActorLocation();

    double GroundHitLocation = GetGroundHitLocation();
    
    BuildLocation.Z = GroundHitLocation + 40.0f;
    return BuildLocation;
}



FRotator UBuildSystem::GetBuildRotation() const
{
    FRotator Rotation = Camera->GetComponentRotation();
    return FRotator(0, FMath::GridSnap(Rotation.Yaw, 90), 0);
}


// Called every frame
void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsBuilding)
    {
        TArray<UInventorySlot*> InventoryArray = CharacterInventory->GetInventoryArray();
        if (InventoryArray.Num() == 0)
        {
            bIsBuilding = false;
            return;
        }
        FVector BuildLocation = GetBuildLocation();
        FRotator BuildRotation = GetBuildRotation();

            if (CurrentItem == nullptr)
            {
                CurrentItem = GetWorld()->SpawnActor<ATurret>(InventoryArray[BuildableIdx]->GetItemData().ActorReference, BuildLocation, BuildRotation);
            }
            else
            {
                CurrentItem->SetActorLocationAndRotation(BuildLocation, BuildRotation);
            }
        }
        else if (CurrentItem != nullptr)
        {
            CurrentItem->Destroy();
            CurrentItem = nullptr;
        }
    }
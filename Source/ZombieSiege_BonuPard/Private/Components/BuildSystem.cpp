#include "Components/BuildSystem.h"
#include "Character/SurvivorCharacter.h"

// Sets default values for this component's properties
UBuildSystem::UBuildSystem()
{
    // Allows the component to receive tick updates every frame
    PrimaryComponentTick.bCanEverTick = true;
}

// Toggles the building mode on or off
void UBuildSystem::ToggleBuildMode()
{
    // Inverts the building state
    bIsBuilding = !bIsBuilding;
}

// Initiates the building process
void UBuildSystem::Build()
{
    FHitResult HitResult;
    FVector BuildLocation = GetBuildLocation(); // Calculates the build location based on player's position and camera direction
    // Exit the function if the inventory is empty
    if (CharacterInventory->GetInventoryArray().Num() == 0) return;
    // Retrieves the ID of the item to be built from the inventory
    FName ItemData = CharacterInventory->GetInventoryArray()[BuildableIdx]->GetItemData().ItemID;

    // Checks for collision at the build location to ensure it's free of obstacles
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        BuildLocation,
        BuildLocation,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeBox(FVector(1, 1, 1))
    );
    
    // If an item is selected, the build mode is active, and no obstacles are found, the item is built
    if (CurrentItem != nullptr && bIsBuilding && !bHit)
    {
        CurrentItem->Build();
        CurrentItem = nullptr; // Resets the current item pointer after building
        // Removes the used item from the inventory
        CharacterInventory->RemoveItem(ItemData, 1);
    }
}

// Cycles through buildable items in the inventory based on user input
void UBuildSystem::SwapBuildable(float AxisValue)
{
    if (!bIsBuilding) return; // Returns if not in building mode

    const TArray<UInventorySlot*> InventoryArray = CharacterInventory->GetInventoryArray();

    if (InventoryArray.Num() == 0) return; // Returns if the inventory is empty
    // Changes the selected item index based on the input direction
    if (AxisValue > 0)
    {
        BuildableIdx -= 1;
    }
    else if (AxisValue < 0)
    {
        BuildableIdx += 1;
    }
    
    // Wraps the index around the inventory array bounds
    if (BuildableIdx < 0) BuildableIdx = InventoryArray.Num() - 1;
    if (BuildableIdx >= InventoryArray.Num()) BuildableIdx = 0;
    
    // Destroys the current item preview when swapping items
    if (CurrentItem != nullptr)
    {
        CurrentItem->Destroy();
        CurrentItem = nullptr;
    }
}

// Initializes component
void UBuildSystem::BeginPlay()
{
    Super::BeginPlay();

    // Retrieves references to necessary components and objects
    PlayerReference = Cast<ASurvivorCharacter>(GetOwner());
    if (PlayerReference)
    {
        Camera = PlayerReference->GetComponentByClass<UCameraComponent>();
        CharacterInventory = PlayerReference->FindComponentByClass<UInventoryComponent>();
        ItemDataTable = CharacterInventory->ItemDataTable; // Optionally retrieve data from an item data table
    }
}

// Calculates the hit location on the ground for building placement
double UBuildSystem::GetGroundHitLocation() const
{
    FVector CameraLocation = Camera->GetComponentLocation();
    FVector Start = CameraLocation;
    FVector End = CameraLocation - FVector(0, 0, BuildDistance); // Adjust the trace length as needed

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerReference); // Ignores the player to avoid self-collision

    // Performs a line trace to find the ground location
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

    if (bHit)
    {
        // Returns the Z coordinate of the hit location if successful
        return HitResult.Location.Z;
    }

    return 0; // Returns 0 if no ground is hit
}

// Determines the build location based on the player's position and camera direction
FVector UBuildSystem::GetBuildLocation() const
{
    FVector Direction = Camera->GetForwardVector() * BuildDistance;
    FVector BuildLocation = Direction + PlayerReference->GetActorLocation();

    double GroundHitLocation = GetGroundHitLocation();
    
    // Adjusts build location's Z coordinate based on ground hit location
    BuildLocation.Z = GroundHitLocation + 40.0f; // Offset to place item above ground
    return BuildLocation;
}

// Calculates the rotation for the building based on the camera's orientation
FRotator UBuildSystem::GetBuildRotation() const
{
    FRotator Rotation = Camera->GetComponentRotation();
    // Snaps the yaw rotation to the nearest 90 degrees for grid alignment
    return FRotator(0, FMath::GridSnap(Rotation.Yaw, 90), 0);
}

// Called every frame
void UBuildSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Handles building placement preview and item selection
    if (bIsBuilding)
    {
        TArray<UInventorySlot*> InventoryArray = CharacterInventory->GetInventoryArray();
        if (InventoryArray.Num() == 0)
        {
            bIsBuilding = false; // Exits building mode if inventory is empty
            return;
        }
        FVector BuildLocation = GetBuildLocation();
        FRotator BuildRotation = GetBuildRotation();

        // Spawns or updates the building preview based on the selected inventory item
        if (CurrentItem == nullptr)
        {
            CurrentItem = GetWorld()->SpawnActor<ATurret>(InventoryArray[BuildableIdx]->GetItemData().ActorReference, BuildLocation, BuildRotation);
        }
        else
        {
            // Updates the location and rotation of the current preview item
            CurrentItem->SetActorLocationAndRotation(BuildLocation, BuildRotation);
        }
    }
    else if (CurrentItem != nullptr)
    {
        // Destroys the preview item if not in building mode
        CurrentItem->Destroy();
        CurrentItem = nullptr;
    }
}

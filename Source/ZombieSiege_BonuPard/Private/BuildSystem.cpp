// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem.h"

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
	}
}

void UBuildSystem::SwapBuildable(float AxisValue)
{
	if (!bIsBuilding) return;
	
	if (AxisValue > 0)
	{
		BuildableIdx -= 1;
	}
	else if (AxisValue < 0)
	{
		BuildableIdx += 1;
	}
	
	if (BuildableIdx < 0) BuildableIdx = Buildables.Num() - 1;
	if (BuildableIdx >= Buildables.Num()) BuildableIdx = 0;
	
	if (CurrentItem != nullptr)
	{
		CurrentItem->Destroy();
		CurrentItem = nullptr;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AxisValue: %f"), AxisValue));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("BuildableIdx: %d"), BuildableIdx));
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
		FVector BuildLocation = GetBuildLocation();
		FRotator BuildRotation = GetBuildRotation();

			if (CurrentItem == nullptr)
			{
				CurrentItem = GetWorld()->SpawnActor<ATurret>(Buildables[BuildableIdx], BuildLocation, BuildRotation);
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Turret.h"
#include "AITurretController.h"
#include "Components/BoxComponent.h"

// Sets default values
ATurret::ATurret()
{
 PrimaryActorTick.bCanEverTick = true;

    // Initialize the RootComponent first
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Initialize and setup CollisionBox
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    // Initialize and setup PreviewMesh
    PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
    PreviewMesh->SetupAttachment(RootComponent);
    PreviewMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    // Initialize and setup Mesh
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetVisibility(false);
    Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

    // Initialize and setup InvalidMesh
    InvalidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InvalidMesh"));
    InvalidMesh->SetupAttachment(RootComponent);
    InvalidMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurret::Sell()
{
	Destroy();
}

void ATurret::Upgrade()
{
	if (TurretLevel != ETurretLevel::Level3)
	{
		TurretLevel = static_cast<ETurretLevel>(static_cast<int>(TurretLevel) + 1);
	}
}

void ATurret::Build()
{
	TurretLevel = ETurretLevel::Level1;
	PreviewMesh->SetVisibility(false);
	Mesh->SetVisibility(true);
	Mesh->SetSimulatePhysics(true);
	AAITurretController* TurretController = GetWorld()->SpawnActor<AAITurretController>(GetActorLocation(), GetActorRotation());
	TurretController->SetupData(GetAIData());
	TurretController->Possess(this);
}

const FAIDataForSightConfig* ATurret::GetAIData() const
{
	if (DataTableReference.IsValid())
	{
		// for testing just pick first row
		FItemData* Data = DataTableReference->FindRow<FItemData>(FName("Classic_Turret"), FString());
		if (Data)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Data found"));
			AIData.AISightRadius = Data->AISightRadius;
			AIData.AILoseSightRadius = Data->AILoseSightRadius;
			AIData.AISightAge = Data->AISightAge;
			AIData.AIFieldOfView = Data->AIFieldOfView;
		} else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No data found"));
		}
	}
	return &AIData;
}

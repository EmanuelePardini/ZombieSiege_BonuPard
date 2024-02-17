// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildables/Turret.h"
#include "Buildables/AITurretController.h"
#include "Components/MoneySystemComponent.h"
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
		SetupLevel(TurretLevel);
	}
}

FText ATurret::Interact(AActor* Interactor)
{
	FText Label;
	if (TurretLevel == ETurretLevel::Level3)
	{
		Label = FText::FromString("Already at max level");
		return Label;
	}
	
	int NextLevelData = ItemData.Levels[static_cast<ETurretLevel>(static_cast<int>(TurretLevel) + 1)].Price;
	bool CanAffordUpgrade = Interactor->GetComponentByClass<UMoneySystemComponent>()->CanAfford(NextLevelData).Get<0>();
	if (CanAffordUpgrade)
	{
		Interactor->GetComponentByClass<UMoneySystemComponent>()->SpendMoney(NextLevelData);
		Upgrade();
		Label = FText::FromString("Upgraded to level " + FString::FromInt(static_cast<int>(TurretLevel)));
		return Label;
	} 
	Label = FText::FromString("Not enough money");
	return Label;
}

void ATurret::SetupTurret()
{
	if (DataTableReference.IsValid())
	{
		FItemData* Data = DataTableReference->FindRow<FItemData>(RowName, FString());
		if (Data)
		{
			ItemData = *Data;
			SetupLevel(TurretLevel);
		}
	}
}

void ATurret::Build()
{
	TurretLevel = ETurretLevel::Level1;
	SetupTurret();
	PreviewMesh->SetVisibility(false);
	Mesh->SetVisibility(true);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None, 10000, true);
	Mesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	AAITurretController* TurretController = GetWorld()->SpawnActor<AAITurretController>(GetActorLocation(), GetActorRotation());
	TurretController->SetupData(GetAIData());
	TurretController->Possess(this);
	
}

void ATurret::SetupLevel(ETurretLevel Level)
{
	LevelData = ItemData.Levels[Level];
}

const FAIDataForSightConfig* ATurret::GetAIData()
{
	if (DataTableReference.IsValid())
	{
		FItemData* Data = DataTableReference->FindRow<FItemData>(RowName, FString());
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



// Fill out your copyright notice in the Description page of Project Settings.

#include "Buildables/AITurretController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAITurretController::AAITurretController()
{
	
		AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

		SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		SightConfig->SightRadius = 1000.0f; // Adjust as needed
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f; // Adjust as needed
		SightConfig->PeripheralVisionAngleDegrees = 360.0f;
		SightConfig->SetMaxAge(5.0f); // Adjust as needed
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f; // Adjust as needed
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	if (AIPerceptionComponent != nullptr) {
		SetPerceptionComponent(*AIPerceptionComponent);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAITurretController::OnPerceptionUpdated);
	}
	
}

void AAITurretController::SetupData(const FAIDataForSightConfig* AISightConfigData)
{
	if (!AISightConfigData) return;

	SightConfig->SightRadius = AISightConfigData->AISightRadius;
	SightConfig->LoseSightRadius = AISightConfigData->AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AISightConfigData->AIFieldOfView;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AISightConfigData->AISightRadius + 20.f;
	SightConfig->SetMaxAge(AISightConfigData->AISightAge);
}

void AAITurretController::UpdateTurretRotation(float DeltaSeconds)
{
	if (CurrentTargetActor)
	{
		FVector TargetLocation = CurrentTargetActor->GetActorLocation();
		FVector MyLocation = GetPawn()->GetActorLocation();
		FVector Direction = TargetLocation - MyLocation;
		Direction.Z = 0;

		FRotator CurrentRotation = GetPawn()->GetActorRotation();
		FRotator TargetRotation = Direction.Rotation();

		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 8.f);

		GetPawn()->SetActorRotation(NewRotation);
	}
}

void AAITurretController::BeginPlay()
{
	Super::BeginPlay();

	if (PerceptionComponent)
	{
		PerceptionComponent->RequestStimuliListenerUpdate();
	}
}

void AAITurretController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateTurretRotation(DeltaSeconds);
}

void AAITurretController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	AActor* ClosestActor = nullptr;
	float MinDistanceSquared = FLT_MAX;

	FVector MyLocation = GetPawn()->GetActorLocation();

	for (AActor* Actor : UpdatedActors)
	{
		if (Actor->ActorHasTag("Player"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player detected"));
		
			float DistanceSquared = FVector::DistSquared(MyLocation, Actor->GetActorLocation());
			if (DistanceSquared < MinDistanceSquared)
			{
				MinDistanceSquared = DistanceSquared;
				ClosestActor = Actor;
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Perception updated"));
	if (ClosestActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ClosestActor->GetName());
	}
	CurrentTargetActor = ClosestActor;
}

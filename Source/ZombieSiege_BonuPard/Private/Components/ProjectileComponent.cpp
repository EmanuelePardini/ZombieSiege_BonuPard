// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProjectileComponent.h"

#include "Character/SurvivorCharacter.h"
#include "Enemies/ZombiesNPC.h"

// Sets default values for this component's properties
UProjectileComponent::UProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	AttachProjectileLine();
}


// Called every frame
void UProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ManageShootTimer(DeltaTime);
}

void UProjectileComponent::ManageShootTimer(float DeltaTime)
{
	ShootTimer+=DeltaTime;
	if(ShootTimer >= ShootDelay)
	{
		CanShoot = true;
		ShootTimer = 0;
	}
}


void UProjectileComponent::Shoot()
{
	if(CanShoot)
	{
		CanShoot = false;
		ActualAmmo--;
		
		//Variables Declaration
		UWorld* Context = GetWorld();
		ASurvivorCharacter* Owner = Cast<ASurvivorCharacter>(GetOwner());
		FVector Start = WeaponComponent->GetComponentLocation();
		FVector End = Start + Owner->GetActorForwardVector() * ProjectileDistance;
		TArray<FHitResult> Hits;

		DrawDebugLine(Context, Start, End, FColor::Red);
	
		Context->LineTraceMultiByProfile(Hits, Start, End, InteractionMask);
		
		for(auto Hit : Hits)
		{
			AActor* HittedActor = Hit.GetActor();
			AZombiesNPC* Enemy = Cast<AZombiesNPC>(HittedActor);
			if(HittedActor != GetOwner())
			{
				if(Enemy)
				{
					Enemy->HealthComponent->DecrementHealth(DamageAmount);
				}
				else
				{
					break;
				}
			}
		}
	}
}

void UProjectileComponent::AttachProjectileLine()
{
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		TArray<UStaticMeshComponent*> Components;
		Owner->GetComponents<UStaticMeshComponent>(Components);

		for (UStaticMeshComponent* Comp : Components)
		{
			if (Comp->GetName() == WeaponComponentName) 
				{
				WeaponComponent = Comp;
				break;
				}
		}
	}
}

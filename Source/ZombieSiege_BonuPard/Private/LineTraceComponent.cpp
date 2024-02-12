// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceComponent.h"

#include "BuyableItem.h"
#include "CustomHUD.h"
#include "Character/SurvivorCharacter.h"
#include "Character/SurvivorController.h"
#include "InputMappingContext.h"


struct FLineTraceResults
{
	TArray<FHitResult> Hits;
	FVector Start;
	FVector End;
	FCollisionQueryParams Params;
};

// Sets default values for this component's properties
ULineTraceComponent::ULineTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULineTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ASurvivorCharacter* Owner = GetOwner<ASurvivorCharacter>())
	{
		UCameraComponent* CameraComponent = Cast<UCameraComponent>(Owner->GetComponentByClass(UCameraComponent::StaticClass()));
		if (CameraComponent)
		{
			Camera = CameraComponent;
		}
	}
}


// Called every frame
void ULineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	IsInteractable();
	
}

void ULineTraceComponent::Interact() const
{
	auto [Hits, _Start, _End, _Params] = DoLineTrace(false);
	ASurvivorCharacter* Owner = GetOwner<ASurvivorCharacter>();
	for (FHitResult Hit: Hits)
	{
		ABuyableItem* Interactable = Cast<ABuyableItem>(Hit.GetActor());
		if (!Interactable) continue;
		Interactable->Buy(Owner);
	}
}

void ULineTraceComponent::IsInteractable() const
{
	UWorld* Context = GetWorld();
	ASurvivorCharacter* Owner = GetOwner<ASurvivorCharacter>();
	if (!Context || !Owner) return;

	auto [Hits, _Start, _End, _Params] = DoLineTrace(false);

	ACustomHUD* HUD = Cast<ACustomHUD>(Context->GetFirstPlayerController()->GetHUD());
	TArray<FEnhancedActionKeyMapping> Mappings = Cast<ASurvivorController>(Owner->GetController())->IMC->GetMappings();
	FString ActionKey;

	for (FEnhancedActionKeyMapping Mapping : Mappings)
	{
		if ( Mapping.Action->GetFName() == FName("Interact"))
		{
			ActionKey = Mapping.Key.GetDisplayName().ToString();
		}
	}

	if (Hits.Num() > 0 && HUD && ActionKey != "")
	{
		HUD->SetTopLeftText(FText::FromString("Press " + ActionKey + " to interact"));
	}
	else if (HUD)
	{
		HUD->SetTopLeftText(FText::FromString(""));
	}
}

FTraces ULineTraceComponent::DoLineTrace(const bool bIsDebug) const
{
	FTraces Traces;
	const UWorld* Context = GetWorld();
	const ASurvivorCharacter* Owner = GetOwner<ASurvivorCharacter>();
	if (!Context || !Owner) return Traces;

	// TODO: Add a way to prevent interaction if character is not facing the interactable
	Traces.Start = Owner->GetActorLocation();
	Traces.Start.Z += Owner->BaseEyeHeight;
	Traces.End = Traces.Start + Camera->GetForwardVector() * InteractionDistance;
	Traces.Params.AddIgnoredActor(Owner);

	#if WITH_EDITOR
		if (bIsDebug)
		{
			DrawDebugLine(Context, Traces.Start, Traces.End, FColor::Red, false, 10.f, 0, 1.f);
		}
	#endif

	Context->LineTraceMultiByProfile(Traces.Hits, Traces.Start, Traces.End, InteractionMask, Traces.Params);
	return Traces;
}

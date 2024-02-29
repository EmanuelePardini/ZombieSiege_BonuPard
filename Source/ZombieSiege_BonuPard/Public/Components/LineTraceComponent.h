#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"

USTRUCT()
struct FTraces
{
	GENERATED_USTRUCT_BODY()
	TArray<FHitResult> Hits;
	FVector Start;
	FVector End;
	FCollisionQueryParams Params;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESIEGE_BONUPARD_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULineTraceComponent();

	UPROPERTY(EditAnywhere)
	float InteractionDistance = 200.f;

	UPROPERTY(EditAnywhere)
	FName InteractionMask = FName("Interactable");

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Interact() const;
	void Interact(const FInputActionValue& Value);
	void Revive(const FInputActionValue& Value);
	void IsInteractable() const;

private:
	/**
	 * Performs a line trace to detect interactable objects.
	 *
	 * @param bIsDebug If true, visualizes the line trace for debugging purposes.
	 * @return FTraces struct containing information about the line trace results (it could just return Hits but you never know).
	 */
	FTraces DoLineTrace(bool bIsDebug = false) const;
};

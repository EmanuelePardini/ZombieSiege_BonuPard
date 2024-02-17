// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IInteractableInterface
 * @brief Interface for interactable objects.
 * @details This interface is used to define the interaction behavior of objects in the game.
 */
class IInteractableInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when the object is interacted with.
	 * @param Interactor The actor that is interacting with the object.
	 */
	virtual FText Interact(AActor* Interactor) = 0;
};
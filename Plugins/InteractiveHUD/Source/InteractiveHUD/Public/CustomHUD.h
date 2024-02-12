// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainUserWidget.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVEHUD_API ACustomHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainUserWidget> WidgetSubclass;

	UPROPERTY()
	UMainUserWidget* MainUserWidget;

	virtual void BeginPlay() override;

public:
	void SetLabelText(const FText& Text);

	void SetTopLeftText(const FText& Text);
};

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
public:
	UPROPERTY()
	UMainUserWidget* MainUserWidget;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainUserWidget> WidgetSubclass;

	UPROPERTY()
	FTimerHandle ClearTextTimerHandle;

	virtual void BeginPlay() override;

public:
	void SetLabelText(const FText& Text);
	void ClearLabelText();

	UFUNCTION(BlueprintCallable)
	void SetTopLeftText(const FText& Text);
	UFUNCTION(BlueprintCallable)
	void SetTopRightText(const int Text);
	UFUNCTION(BlueprintCallable)
	void SetBottomLeftText(const int Text);
	UFUNCTION(BlueprintCallable)
	void SetBottomRightText(const int Text);
};

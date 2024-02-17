// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MainUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVEHUD_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* InteractionLabel;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* TopLeftLabel;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* TopRightLabel;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* BottomLeftLabel;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* BottomRightLabel;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* BottomRightLabel2;

public:
	void SetInteractionLabelContent(const FText& Text);

	void SetTopLeftTextContent(const FText& Text);
	void SetTopRightTextContent(const FText& Text);
	void SetBottomLeftTextContent(const FText& Text);
	void SetBottomRightTextContent(const FText& Text);
	void SetBottomRightTextContent2(const FText& Text);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomHUD.h"

void ACustomHUD::BeginPlay()
{
	Super::BeginPlay();
	MainUserWidget = CreateWidget<UMainUserWidget>(GetWorld(), WidgetSubclass);
	MainUserWidget->AddToViewport();
}

void ACustomHUD::SetLabelText(const FText& Text)
{
	if(MainUserWidget) MainUserWidget->SetInteractionLabelContent(Text);
}

void ACustomHUD::SetTopLeftText(const FText& Text)
{
	if(MainUserWidget) MainUserWidget->SetTopLeftTextContent(Text);
}

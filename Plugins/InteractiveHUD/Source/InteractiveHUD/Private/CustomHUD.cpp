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
	if(MainUserWidget) {
		MainUserWidget->SetInteractionLabelContent(Text);
		GetWorld()->GetTimerManager().ClearTimer(ClearTextTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ClearTextTimerHandle, this, &ACustomHUD::ClearLabelText, 3.0f, false);}
}

void ACustomHUD::ClearLabelText()
{
	if (MainUserWidget) MainUserWidget->SetInteractionLabelContent(FText::GetEmpty());
}

void ACustomHUD::SetTopLeftText(const FText& Text)
{
	if(MainUserWidget) MainUserWidget->SetTopLeftTextContent(Text);
}

void ACustomHUD::SetTopRightText(const int Text)
{
	if(MainUserWidget) MainUserWidget->SetTopRightTextContent(FText::FromString(FString::FromInt(Text)));
}

void ACustomHUD::SetBottomLeftText(const int Text)
{
	if(MainUserWidget) MainUserWidget->SetBottomLeftTextContent(FText::FromString(FString::FromInt(Text)));
}

void ACustomHUD::SetBottomRightText(const int Text)
{
	if(MainUserWidget) MainUserWidget->SetBottomRightTextContent(FText::FromString(FString::FromInt(Text)));
}

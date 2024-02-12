// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"

void UMainUserWidget::SetInteractionLabelContent(const FText& Text)
{
	InteractionLabel->SetText(Text);
}

void UMainUserWidget::SetTopLeftTextContent(const FText& Text)
{
	TopLeftLabel->SetText(Text);
}

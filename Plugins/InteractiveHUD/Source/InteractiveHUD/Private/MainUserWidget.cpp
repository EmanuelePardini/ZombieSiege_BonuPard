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

void UMainUserWidget::SetTopRightTextContent(const FText& Text)
{
	TopRightLabel->SetText(Text);
}

void UMainUserWidget::SetBottomLeftTextContent(const FText& Text)
{
	BottomLeftLabel->SetText(Text);
}

void UMainUserWidget::SetBottomRightTextContent(const FText& Text)
{
	BottomRightLabel->SetText(Text);
}

void UMainUserWidget::SetBottomRightTextContent2(const FText& Text)
{
	BottomRightLabel2->SetText(Text);
}

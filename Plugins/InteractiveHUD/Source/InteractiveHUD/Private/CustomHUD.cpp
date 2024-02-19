// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomHUD.h"

#include "Character/SurvivorController.h"
#include "Kismet/GameplayStatics.h"

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

void ACustomHUD::InitTriggerReference()
{
	ASurvivorController* Controller = Cast<ASurvivorController>(GetOwner());
	if(!Controller) return;
	ASurvivorCharacter* Player = Cast<ASurvivorCharacter>(Controller->GetPawn());
	if(!Player) return;
	
	MoneyTrigger = Player->MoneySystemComponent;
	MoneyTrigger->OnMoneyChanged.AddDynamic(this, &ACustomHUD::SetTopRightText);
	MoneyTrigger->SetupInitialMoney();
	
	HealthTrigger = Player->HealthComponent;
	HealthTrigger->OnHealthChanged.AddDynamic(this, &ACustomHUD::SetBottomRightText2);
	HealthTrigger->SetupInitialHealth();

	ProjectileTrigger = Player->ProjectileComponent;
	ProjectileTrigger->OnAmmoChanged.AddDynamic(this, &ACustomHUD::SetBottomRightText);
	ProjectileTrigger->SetupInitialAmmo();

	RoundTrigger = Cast<AZombiesRounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AZombiesRounds::StaticClass()));
	RoundTrigger->OnRoundChange.AddDynamic(this, &ACustomHUD::SetBottomLeftText);
	RoundTrigger->SetupInitialRound();
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

void ACustomHUD::SetBottomRightText2(const int Text)
{
	if(MainUserWidget) MainUserWidget->SetBottomRightTextContent2(FText::FromString(FString::FromInt(Text)));
}
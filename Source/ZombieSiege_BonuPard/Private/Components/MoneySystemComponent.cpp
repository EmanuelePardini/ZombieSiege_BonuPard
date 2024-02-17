

#include "Components/MoneySystemComponent.h"

#include "CustomHUD.h"
#include "Character/SurvivorCharacter.h"
#include "Character/SurvivorController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMoneySystemComponent::UMoneySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMoneySystemComponent::SetupInitialMoney()
{
	Money = InitialMoney;
}

void UMoneySystemComponent::SetupUI()
{
	UWorld* Context = GetWorld();
	ASurvivorCharacter* Player = Cast<ASurvivorCharacter>(GetOwner());
	ASurvivorController* Controller = Cast<ASurvivorController>(Player->GetController());
	if (!Context || !Player || !Controller) return;
	ACustomHUD* HUD = Controller->PlayerHUD;
	if (!HUD) return;
	OnMoneyChanged.AddDynamic(HUD, &ACustomHUD::SetTopRightText);
	OnMoneyChanged.Broadcast(Money);
}

void UMoneySystemComponent::InitTriggerReference()
{
	RoundsTrigger = Cast<AZombiesRounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AZombiesRounds::StaticClass()));
	if(RoundsTrigger)
	{
		RoundsTrigger->OnZombieKilled.AddDynamic(this, &UMoneySystemComponent::AddMoney);
		RoundsTrigger->OnRoundEnd.AddDynamic(this, &UMoneySystemComponent::AddMoney);
	}
}

void UMoneySystemComponent::AddMoney(const int Amount)
{
	Money += Amount;
	OnMoneyChanged.Broadcast(Money);
}

void UMoneySystemComponent::RemoveMoney(const int Amount)
{
	Money -= Amount;
	OnMoneyChanged.Broadcast(Money);
}

void UMoneySystemComponent::SpendMoney(const int Amount)
{
	RemoveMoney(Amount);
	OnMoneyChanged.Broadcast(Money);
}

TTuple<bool, int> UMoneySystemComponent::CanAfford(const int Amount) const
{
	return TTuple<bool, int>(Money >= Amount, Money - Amount);
}

// Called when the game starts
void UMoneySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInitialMoney();
	InitTriggerReference();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMoneySystemComponent::SetupUI, 5.0f, false);
}


// Called every frame
void UMoneySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

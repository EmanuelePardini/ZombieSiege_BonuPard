

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

bool UMoneySystemComponent::CanAfford(const int Amount)
{
	if (Money >= Amount) return true;
	return false;
}

// Called when the game starts
void UMoneySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	InitTriggerReference();

}


// Called every frame
void UMoneySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

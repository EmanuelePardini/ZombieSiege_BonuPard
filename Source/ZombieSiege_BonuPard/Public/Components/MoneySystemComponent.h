#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemies/ZombiesRounds.h"
#include "MoneySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESIEGE_BONUPARD_API UMoneySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMoneySystemComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Economy")
	int Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy")
	int InitialMoney = 0;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int, NewMoneyAmount);
	FOnMoneyChanged OnMoneyChanged;
	UPROPERTY(EditAnywhere)
	AZombiesRounds* RoundsTrigger;
	UPROPERTY(EditAnywhere, Category = "Setup")
	int KillReward = 50;
	UPROPERTY(EditAnywhere, Category = "Setup")
	int RoundReward = 1000;

	void SetupInitialMoney();

	void SetupUI();
    void InitTriggerReference();
	UFUNCTION()
	void AddMoney(int Amount);
	UFUNCTION()
	void RemoveMoney(int Amount);
	UFUNCTION()
	void  SpendMoney(int Amount);
	TTuple<bool, int> CanAfford(int Amount) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	int GetMoney() const { return Money; }
};
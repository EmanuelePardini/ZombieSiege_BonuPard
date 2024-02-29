// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege_BonuPard/Public/Character/SurvivorCharacter.h"

#include "ZombieSiege_GameMode.h"
#include "Components/BuildSystem.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/SpawnInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASurvivorCharacter::ASurvivorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Movement Settings
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	//Audio Settings
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;

	//Components Settings
	ProjectileComponent = CreateDefaultSubobject<UProjectileComponent>("ProjectileComponent");
	LineTraceComponent = CreateDefaultSubobject<ULineTraceComponent>("LineTraceComponent");
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	BuildSystemComponent = CreateDefaultSubobject<UBuildSystem>("BuildSystemComponent");
	MoneySystemComponent = CreateDefaultSubobject<UMoneySystemComponent>("MoneySystemComponent");
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->MaxHealth = 50;
	HealthComponent->Health = 50;
}

// Called when the game starts or when spawned
void ASurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Animation Settings
	Animations = Cast<USurvivorAnim>(GetMesh()->GetAnimInstance());
}

void ASurvivorCharacter::Move(const FInputActionValue& Value)
{
	if(!IsShooting && !IsReloading && !IsDied)
	{
		// Obtains the controller's rotation and create a rotation on the yaw axis
		FVector2d MovementValue = Value.Get<FVector2d>();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Gets the forward and right directions based on the Yaw rotation
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Adds movement input along the forward axis (ForwardDirection) and the right axis (RightDirection)
		AddMovementInput(ForwardDirection, MovementValue.Y);
		AddMovementInput(RightDirection, MovementValue.X);
	}
}

void ASurvivorCharacter::Look(const FInputActionValue& Value)
{
	if(!IsDied)
	{
		FVector2d LookValue = Value.Get<FVector2d>();
	
		// Adds input to control the rotation
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void ASurvivorCharacter::DoJump(const FInputActionValue& Value)
{
	if(!IsCrouched && !IsAiming && !IsDied)
	{
		Jump();
	}
}

void ASurvivorCharacter::Run(const FInputActionValue& Value)
{
	if(!IsCrouched && !IsAiming && !IsDied)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		Animations->IsRunning = true;
		IsRunning = true;
	}
}

void ASurvivorCharacter::EndRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	Animations->IsRunning = false;
	IsRunning = false;
}

void ASurvivorCharacter::DoCrouch()
{
	if(!IsRunning && !IsAiming && !IsDied)
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
		IsCrouched = true;
		Animations->IsCrouched = true;
	}
}

void ASurvivorCharacter::DoUnCrouch()
{
	if(!IsAiming && !IsDied)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		IsCrouched = false;
		Animations->IsCrouched = false;
	}
}

void ASurvivorCharacter::Aim(const FInputActionValue& Value)
{
	if(!IsRunning && !IsReloading && !IsDied)
	{
		IsAiming = true;
		Animations->IsAiming = true;
	}
}

void ASurvivorCharacter::StopAim(const FInputActionValue& Value)
{
	//First stop shooting
	AudioComponent->Stop();
	IsShooting = false;
	Animations->IsShooting = false;
	
	IsAiming = false;
	Animations->IsAiming = false;
}

void ASurvivorCharacter::Shoot(const FInputActionValue& Value)
{
	if(IsAiming && ProjectileComponent->ActualAmmo > 0 && !IsDied)
	{ //If is aiming and has ammo then can shoot
		if(!IsShooting) //If is starting to shoot now play the sound only one time
		{
			AudioComponent->SetSound(ShootingSound);
			AudioComponent->Play();
		}
		IsShooting = true;
		Animations->IsShooting = true;
		ProjectileComponent->Shoot();

	}
	else
	{ //If has no ammo need to recharge
		Reload(true);
	}
}

void ASurvivorCharacter::StopShoot(const FInputActionValue& Value)
{
		AudioComponent->Stop();
		IsShooting = false;
		Animations->IsShooting = false;
}

void ASurvivorCharacter::Reload(const FInputActionValue& Value)
{
	if(ProjectileComponent->ActualAmmo < ProjectileComponent->LoaderAmmo && !IsDied)
	{
		StopShoot(true);
		StopAim(true);
		IsReloading = true;
		Animations->IsReloading = true;
	}
}

void ASurvivorCharacter::ManageReload(float DeltaTime)
{
	ReloadTimer += DeltaTime;
	if(ReloadTimer >= ReloadDelay)
	{
		ProjectileComponent->Recharge();
		Animations->IsReloading = false;
		IsReloading = false;
		ReloadTimer = 0;
	}
}

void ASurvivorCharacter::ToggleBuild(const FInputActionValue& Value)
{
	if(!IsDied)
	{
		if (auto* BuildSystem = GetComponentByClass<UBuildSystem>()) BuildSystem->ToggleBuildMode();
	}
	
}

void ASurvivorCharacter::Build(const FInputActionValue& Value)
{
	if(!IsDied)
	{
		if (auto* BuildSystem = GetComponentByClass<UBuildSystem>()) BuildSystem->Build();
	}
	
}

void ASurvivorCharacter::SwapBuildable(const FInputActionValue& InputActionValue)
{
	if(!IsDied)
	{
		if (auto* BuildSystem = GetComponentByClass<UBuildSystem>())  BuildSystem->SwapBuildable(InputActionValue.Get<float>());
	}
	
}

void ASurvivorCharacter::Interactor(const FInputActionValue& Value)
{
	if(!IsDied)
	{
		GetComponentByClass<ULineTraceComponent>()->Interact();
	}
}

void ASurvivorCharacter::Revive(const FInputActionValue& Value)
{
	if(!IsDied)
	{
		GetComponentByClass<ULineTraceComponent>()->Revive(Value);
	}
}

FText ASurvivorCharacter::Interact(AActor* Interactor)
{
	FText Label;
	ASurvivorCharacter* Friend = Cast<ASurvivorCharacter>(Interactor);
	GetWorld()->GetTimerManager().SetTimer(IsRevivingTimer, this, &ASurvivorCharacter::DecreaseRevive, 1.f, true);
	if (Friend->IsDied)
	{
		if (ReviveProgression == 0)
		{
			ReviveProgression++;
		} else if (ReviveProgression < ReviveTotal)
		{
			ReviveProgression++;
		} else
		{
			Spawn(GetActorLocation());
		}
		return FText::FromString("Revive");
	}
	return Label;
}

void ASurvivorCharacter::DecreaseRevive()
{
	if (ReviveProgression > 0) ReviveProgression--;
	if (ReviveProgression == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(IsRevivingTimer);
	}
}

void ASurvivorCharacter::ManageCoop(const FInputActionValue& Value)
{
		AZombieSiege_GameMode* GameMode = Cast<AZombieSiege_GameMode>(GetWorld()->GetAuthGameMode());
		if(GameMode) GameMode->CheckCoop();           
}

// Called every frame
void ASurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//For animations
	if(Animations)
	{
		Animations->Animate(this);
	}
	//For automatic Actions
	if(IsReloading)
	{
		ManageReload(DeltaTime);
	}

}

void ASurvivorCharacter::Spawn(FVector Location)
{
	ISpawnInterface::Spawn(Location);
	IsDied = false;
	Animations->IsDead = false;
	HealthComponent->SetupInitialHealth();
}

void ASurvivorCharacter::Die()
{
	ISpawnInterface::Die();

	IsDied = true;
	Animations->IsDead = true;
	
	UWorld* World = GetWorld();
	
	if (World)
	{
		AZombieSiege_GameMode* GameMode = Cast<AZombieSiege_GameMode>(World->GetAuthGameMode());
		if(GameMode && GameMode->IsCoop)
		{
			if(!IsAnyOneAlive()) UGameplayStatics::OpenLevel(this, "GameOverMap");
		}
		else
		{
			UGameplayStatics::OpenLevel(this, "GameOverMap");
		}
		
	}
}

bool ASurvivorCharacter::IsAnyOneAlive()
{
	UWorld* World = GetWorld();
	TArray<AActor*> FoundSurvivorCharacters;
	bool IsAnyoneAlive = false;
	
	UGameplayStatics::GetAllActorsOfClass(World, ASurvivorCharacter::StaticClass(), FoundSurvivorCharacters);
				
	for (AActor* Actor : FoundSurvivorCharacters)
	{
		ASurvivorCharacter* SurvivorCharacter = Cast<ASurvivorCharacter>(Actor);
		if (!SurvivorCharacter->IsDied)
		{
			IsAnyoneAlive = true;
		}
	}

	return IsAnyoneAlive;
}

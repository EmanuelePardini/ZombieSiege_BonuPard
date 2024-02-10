// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSiege_BonuPard/Public/Character/SurvivorCharacter.h"

#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/SpawnInterface.h"

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
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	HealthComponent->MaxHealth = 50.f;
	HealthComponent->Health = 50.f;
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
	if(!IsShooting && !IsReloading)
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
	FVector2d LookValue = Value.Get<FVector2d>();
	
	// Adds input to control the rotation
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void ASurvivorCharacter::DoJump(const FInputActionValue& Value)
{
	if(!IsCrouched && !IsAiming)
	{
		Jump();
	}
}

void ASurvivorCharacter::Run(const FInputActionValue& Value)
{
	if(!IsCrouched && !IsAiming)
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

void ASurvivorCharacter::Crouch(const FInputActionValue& Value)
{
	if(!IsRunning && !IsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
		IsCrouched = true;
		Animations->IsCrouched = true;
	}
}

void ASurvivorCharacter::UnCrouch(const FInputActionValue& Value)
{
	if(!IsAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		IsCrouched = false;
		Animations->IsCrouched = false;
	}
}

void ASurvivorCharacter::Aim(const FInputActionValue& Value)
{
	if(!IsRunning && !IsReloading)
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
	if(IsAiming && ProjectileComponent->ActualAmmo > 0)
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
	if(ProjectileComponent->ActualAmmo < ProjectileComponent->LoaderAmmo)
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
		ProjectileComponent->ActualAmmo = 30;
		Animations->IsReloading = false;
		IsReloading = false;
		ReloadTimer = 0;
	}
}

void ASurvivorCharacter::Interact(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString(TEXT("To implement Interact")));
}

void ASurvivorCharacter::Drop(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString(TEXT("To implement Drop")));
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
}

void ASurvivorCharacter::Die()
{
	ISpawnInterface::Die();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "AttackComponent.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Misc/OutputDeviceNull.h"
#include "Enemy.h"
#include "Twiglide_Instance.h"
#include "Engine/World.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// set the pitch min and max
	pitchMin = -50.0f;
	pitchMax = 10.0f;

	// set the dash Mechanic values
	canDash = true;
	isDashing = false;
	dashDistance = 6000.0f;
	dashCooldown = 1.0f;
	dashStop = 0.1f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	timeInCombo = timeBetweenCombo;
}

void APlayerCharacter::CheckScalarValue(float value, float deltaTime)
{
	Super::CheckScalarValue(value, deltaTime);

	if (value <= 0)
		isHit = false;
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Limit pitch of Camera
	APlayerCameraManager* const cam_manager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	cam_manager->ViewPitchMin = pitchMin;
	cam_manager->ViewPitchMax = pitchMax;

	checkPointLocation = GetActorLocation();

	targetedEnemy = nullptr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemies);
}

/** Called function for dash*/
void APlayerCharacter::Dash()
{
	if (canDash && !isDead)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		if (targetLocked)
		{
			FRotator targetRotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetedEnemy->GetActorLocation());
			targetRotate.Pitch = 0.f;
			SetActorRotation(targetRotate);
			LaunchCharacter((targetedEnemy->GetActorLocation() - GetActorLocation())* 10, true, true);
			GetWorldTimerManager().SetTimer(unusedHandle, this, &APlayerCharacter::StopTargetDash, dashStop, false);
		}
		else
		{
			LaunchCharacter(this->GetActorForwardVector() * dashDistance
								, true
								, true);
			GetWorldTimerManager().SetTimer(unusedHandle, this, &APlayerCharacter::StopDashing, dashStop, false);
		}
		canDash = false;
		isDashing = true;
	}
}

void APlayerCharacter::StopTargetDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(unusedHandle, this, &APlayerCharacter::ResetDash, dashCooldown, false);
	isDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
}

void APlayerCharacter::StopDashing()
{
	GetWorldTimerManager().SetTimer(unusedHandle, this, &APlayerCharacter::ResetDash, dashCooldown, false);
	isDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
}

void APlayerCharacter::ResetDash()
{
	canDash = true;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isChargingAttack)
	{
		chargeAttackTimer += DeltaTime;
		if (chargeAttackTimer >= chargeAttackTime)
		{
			isChargingAttack = false;
			isAttackCharge = true;
			chargeAttackTimer = false;
		}
	}

	if (targetLocked)
		RinterpCamera();
	
	if (timeInCombo <= 0)
		numberofHits = 0;
	else
		timeInCombo -= DeltaTime;
	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// set up dash key bindings
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

	//set up attack binding
	PlayerInputComponent->BindAction("LightAttack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("LightAttack", IE_Released, this, &APlayerCharacter::StopAttack);

	PlayerInputComponent->BindAction("HeavyAttack", IE_Pressed, this, &APlayerCharacter::HeavyAttack);
	PlayerInputComponent->BindAction("HeavyAttack", IE_Released, this, &APlayerCharacter::AttackRelease);

	//Parry
	PlayerInputComponent->BindAction("Parry", IE_Pressed, this, &APlayerCharacter::Block);
	PlayerInputComponent->BindAction("Parry", IE_Released, this, &APlayerCharacter::StopBlocking);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);


	// Target inputs //
	
	PlayerInputComponent->BindAction("Target", IE_Pressed, this, &APlayerCharacter::Target);
	PlayerInputComponent->BindAction("Target", IE_Released, this, &APlayerCharacter::StopTarget);
}

void APlayerCharacter::RinterpCamera()
{
	FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), targetedEnemy->GetActorLocation());
	FRotator tempPawnRotator = GetControlRotation();
	FRotator lookAtInterp = UKismetMathLibrary::RInterpTo(tempPawnRotator, lookAtRotator, GetWorld()->GetDeltaSeconds(), 7.0f);
	GetController()->SetControlRotation(UKismetMathLibrary::MakeRotator(tempPawnRotator.Roll, lookAtInterp.Pitch, lookAtInterp.Yaw));

}

TArray<AEnemy*> APlayerCharacter::GetAliveEnemies()
{
	TArray<AEnemy*> tempArray;
	for (AActor * currentEnemy : enemies)
	{
		AEnemy* enemy = Cast<AEnemy>(currentEnemy);
		if (enemy && !enemy->isDead)
			tempArray.Add(enemy);
	}
	return tempArray;
}

void APlayerCharacter::Target()
{
	TArray<AEnemy*> aliveEnemies = GetAliveEnemies();
	float dist = FLT_MAX;
	float distTemp;
	
	for (AEnemy* currentEnemy : aliveEnemies)
	{
		distTemp = FVector::Dist(currentEnemy->GetActorLocation(), GetActorLocation());
		if (distTemp < dist && distTemp <= maxLockRange)
		{
			dist = distTemp;
			targetedEnemy = currentEnemy;
		}
	}
	if (targetedEnemy)
	{
		targetLocked = true;
		targetedEnemy->isTargeted = true;
	}
		
}

void APlayerCharacter::StopTarget()
{
	targetLocked = false;

	if (targetedEnemy)
	{
		targetedEnemy->isTargeted = false;
		targetedEnemy = nullptr;
	}

	
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	if (isDead)
		return;

	UTwiglide_Instance* gameInstance = Cast<UTwiglide_Instance>(GetGameInstance());
		
	if (!targetLocked)
	{
		// calculate delta for this frame from the rate information
		if (gameInstance && gameInstance->bInverseXAxis)
			AddControllerYawInput(-Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		else
			AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	if (isDead)
		return;

	UTwiglide_Instance* gameInstance = Cast<UTwiglide_Instance>(GetGameInstance());

	// calculate delta for this frame from the rate information
	if (gameInstance && gameInstance->bInverseYAxis)
		AddControllerPitchInput(-Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	else
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !isDead)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !isDead)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TakeDamage(int damageTaken)
{
	if (!isHit)
	{
		Super::TakeDamage(damageTaken);
		isHit = true;
		if (isDead)
		{
			FOutputDeviceNull ar;
			CallFunctionByNameWithArguments(TEXT("EventDeathPlayer"), ar, NULL, true);
		}
	}
}

void APlayerCharacter::Attack()
{

	if (canCombo)
	{
		GetWorldTimerManager().ClearTimer(timerHandler);
		comboCounter++;
	}
	
	Super::Attack();
}

void APlayerCharacter::HeavyAttack()
{
	isChargingAttack = true;
}

void APlayerCharacter::AttackRelease()
{
	Super::HeavyAttack();

	if (isAttackCharge)
		damage = heavyDamage * 2;
	
	isChargingAttack = false;
	chargeAttackTimer = 0.f;
}

void APlayerCharacter::Block()
{
	isDefending = true;
}

void APlayerCharacter::StopBlocking()
{
	isDefending = false;
}

void APlayerCharacter::StartCombo()
{
	canCombo = true;
}

void APlayerCharacter::StopCombo()
{
	canCombo = false;
	comboCounter = 0;
}

void APlayerCharacter::StopAttack()
{
	Super::StopAttack();

	//Reset combo if it's the last combo
	if (comboCounter >= 2)
	{
		canCombo = false;
		comboCounter = 0;
	}
}

void APlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Max() != 0
		&& OtherActor->Tags[0] == "Enemy" && !ActorHasTag("Enemy"))
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);

		if (!enemy->isDead)
		{

			if (isAttackCharge)
			{
				FVector launch = { 0.0, 0.0f, 1000.0f };
				enemy->LaunchCharacter(launch, true, true);
				targetLocked = true;
				targetedEnemy = enemy;
			}

			enemy->isHit = true;
			enemy->TakeDamage(damage);

			GetWorld()->GetTimerManager().SetTimer(timerHandler, enemy, &AGenericCharacter::freezeMovemnent, freezePosition, false);
			GetWorld()->GetTimerManager().SetTimer(timerHandlerFreezeMovement, this, &AGenericCharacter::freezeMovemnent, freezePosition, false);

			enemy->isInAirCombat = true;
			enemy->airCombatTimer = 0.0f;

			isInAirCombat = true;
			airCombatTimer = 0.0f;

			enemy->TakeDamage(damage);
		}

		if (timeInCombo <= 0)
		{
			numberofHits = 1;
		}
		else
			numberofHits++;

		timeInCombo = timeBetweenCombo;
	}
}
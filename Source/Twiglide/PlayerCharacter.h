// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TWIGLIDE_API APlayerCharacter : public AGenericCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	bool canDash;

	UPROPERTY()
	FTimerHandle unusedHandle;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Camera view pitch min. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float pitchMin;

	/** Camera view pitch max. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float pitchMax;

	/** Distance the dash is traveling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DashMechanic)
	float dashDistance;

	/** Time you qre effectively dashing*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DashMechanic)
	float dashStop;

	/** Cooldown between dashes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DashMechanic)
	float dashCooldown;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float hitDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool targetLocked;

	UFUNCTION(BlueprintCallable)
	void DisableMouseInput();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* targetedEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float hitCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float chargeAttackTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float chargeAttackTimer = 0.0f;

	bool isHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isChargingAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isDefending = false;

	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for the Dash input */
	void Dash();

	/** exclusif call for the target dash */
	void StopTargetDash();

	/** Called for stoping the dash */
	void StopDashing();

	/** Set canDash to true onceAgain. */
	void ResetDash();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeDamage(int damage) override;

	virtual void HeavyAttack() override;

	void AttackRelease();

	void Block();

	void StopBlocking();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

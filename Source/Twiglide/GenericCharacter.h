// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericCharacter.generated.h"

UENUM(BlueprintType)
enum EAttackType
{
	Light,
	Heavy
};


UCLASS()
class TWIGLIDE_API AGenericCharacter : public ACharacter
{
	GENERATED_BODY()

private:

protected:
	UPROPERTY(EditAnywhere, Category = Attacks, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* attackBox;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	int life;

	int damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int lightDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int heavyDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	int maxLife = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	TEnumAsByte<EAttackType> attackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool heavyAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isAttackCharge = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this character's properties
	AGenericCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage(int damage);

	virtual void Attack();

	virtual void HeavyAttack();

	UFUNCTION(BlueprintCallable, Category = "Class Functions")
	virtual void StopAttack();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);
};

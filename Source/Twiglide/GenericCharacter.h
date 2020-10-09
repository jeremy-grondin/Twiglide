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
	int life;

protected:
	UPROPERTY(EditAnywhere, Category = Attacks, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* attackBox;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	int maxLife = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	TEnumAsByte<EAttackType> attackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	bool isAttacking = false;

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

	void TakeDamage(int damage);

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);
};

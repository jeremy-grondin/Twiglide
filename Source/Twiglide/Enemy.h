// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericCharacter.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class E_EnemyType: uint8
{
	SwordShield UMETA(DisplayName = "SwordAndShield"),
	Bow UMETA(DisplayName = "Bow"),
	Scout UMETA(DisplayName = "Scout"),
	Mannequin UMETA(DisplayName = "Mannequin")
};

UCLASS()
class TWIGLIDE_API AEnemy : public AGenericCharacter
{
	GENERATED_BODY()

private:
	
public:
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	E_EnemyType enemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float minDistanceToPlayer = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float maxDistanceToPlayer = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float attackDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float attackRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float attackCooldown = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float attackDelay = 1.0f;

	float attackTimer = 0.0f;

	bool canAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isTargeted = false;

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(int damage) override;
};

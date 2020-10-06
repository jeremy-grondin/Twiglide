// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class TWIGLIDE_API AEnemy : public AGenericCharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float minDistanceToPlayer = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristic)
	float maxDistanceToPlayer = 300.f;

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

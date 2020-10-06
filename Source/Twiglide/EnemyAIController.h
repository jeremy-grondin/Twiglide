// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TWIGLIDE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	ACharacter* player;
	class AEnemy* enemy;

protected:
	void Tick(float DeltaTime) override;

public:
	void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI)
	class UBehaviorTree* behaviorTree;
};

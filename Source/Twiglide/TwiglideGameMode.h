// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwiglideGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TWIGLIDE_API ATwiglideGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TArray<AActor*> enemies;

	void ChooseAttacker();

	int targetActorIndex;

	FTimerHandle timerHandler;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristic")
	float attackDelayTime = 2.0f;

	ATwiglideGameMode();

	void BeginPlay() override;

	void Tick(float DeltaSecond) override;
};

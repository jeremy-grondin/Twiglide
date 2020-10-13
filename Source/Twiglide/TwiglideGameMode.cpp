// Fill out your copyright notice in the Description page of Project Settings.


#include "TwiglideGameMode.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

ATwiglideGameMode::ATwiglideGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATwiglideGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemies);
	GetWorldTimerManager().SetTimer(timerHandler, this, &ATwiglideGameMode::ChooseAttacker, attackDelayTime, true, attackDelayTime);
}

void ATwiglideGameMode::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
}

void ATwiglideGameMode::ChooseAttacker()
{
	if (enemies.Max() != 0)
	{
		targetActorIndex = FMath::RandRange(0, enemies.Num() - 1);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(targetActorIndex));

		AEnemy* targetEnemy = Cast<AEnemy>(enemies[targetActorIndex]);

		if (targetEnemy != nullptr && !targetEnemy->isDead)
			targetEnemy->canAttack = true;
	}
}
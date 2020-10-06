// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_LookAtPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Enemy.h"
#include "AIController.h"

EBTNodeResult::Type UMyBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();

	FVector enemyPos = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	playerPos.Z = enemyPos.Z = 0;

	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(enemyPos, playerPos);
	

	enemy->SetActorRotation(rotation);

	return EBTNodeResult::Succeeded;
}
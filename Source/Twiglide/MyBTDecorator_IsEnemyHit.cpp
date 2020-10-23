// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_IsEnemyHit.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "EnemyAIController.h"

bool UMyBTDecorator_IsEnemyHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	return enemy->isHit;
}
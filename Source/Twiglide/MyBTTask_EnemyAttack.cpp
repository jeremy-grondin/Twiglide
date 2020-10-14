// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_EnemyAttack.h"
#include "Components/PrimitiveComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "Enemy.h"

EBTNodeResult::Type UMyBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	enemy->isAttacking = true;
	enemy->canAttack = true;
	enemy->Attack();

	return EBTNodeResult::Succeeded;
}
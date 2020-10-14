// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_CanEnemyAttack.h"
#include "Components/PrimitiveComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "Enemy.h"

bool UMyBTDecorator_CanEnemyAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	FVector enemyPos = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	return (playerPos - enemyPos).Size() <= enemy->attackDistance && !enemy->canAttack;
}
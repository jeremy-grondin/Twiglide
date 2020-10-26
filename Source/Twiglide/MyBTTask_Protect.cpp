// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Protect.h"
#include "Components/PrimitiveComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "Enemy.h"

EBTNodeResult::Type UMyBTTask_Protect::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	enemy->isDefending = true;

	return EBTNodeResult::Succeeded;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_EnemyMoveTo.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PrimitiveComponent.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Enemy.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UMyBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FVector movingPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);

	OwnerComp.GetAIOwner()->MoveToLocation(movingPos);

	return EBTNodeResult::Succeeded;
}
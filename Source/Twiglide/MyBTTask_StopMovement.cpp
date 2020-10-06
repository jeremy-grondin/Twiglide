// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_StopMovement.h"
#include "AIController.h"

EBTNodeResult::Type UMyBTTask_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->StopMovement();

	return EBTNodeResult::Succeeded;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "MyBTDecorator_CanEnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class TWIGLIDE_API UMyBTDecorator_CanEnemyAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

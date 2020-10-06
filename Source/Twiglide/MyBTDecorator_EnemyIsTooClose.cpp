// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_EnemyIsTooClose.h"
#include "Enemy.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

bool UMyBTDecorator_EnemyIsTooClose::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	float lenght = (enemy->GetActorLocation() - player->GetActorLocation()).Size();

	return lenght <= enemy->minDistanceToPlayer;
}
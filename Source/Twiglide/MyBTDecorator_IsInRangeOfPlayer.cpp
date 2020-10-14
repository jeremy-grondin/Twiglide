// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator_IsInRangeOfPlayer.h"
#include "Engine/World.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

bool UMyBTDecorator_IsInRangeOfPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AEnemy* enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	return (enemy->GetActorLocation() - player->GetActorLocation()).Size() <= enemy->attackDistance;
}
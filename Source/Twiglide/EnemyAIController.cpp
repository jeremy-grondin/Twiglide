// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/World.h"
#include "Enemy.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	this->RunBehaviorTree(behaviorTree);

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	enemy = Cast<AEnemy>(GetPawn());
	Blackboard->SetValueAsEnum("EnemyType", (int8)enemy->enemyType);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	FVector awayPos = (2 * enemy->GetActorLocation()) - player->GetActorLocation();

	Blackboard->SetValueAsVector("AwayPos", awayPos);
	Blackboard->SetValueAsVector("PlayerPos", player->GetActorLocation());
}
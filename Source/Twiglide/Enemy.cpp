// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canAttack && !isHit)
	{
		attackTimer += DeltaTime;
		if (attackTimer >= attackDelay)
		{
			Attack();
			attackTimer = 0.f;
			canAttack = false;
		}
	}

}

void AEnemy::TakeDamage(int damageTaken)
{
	Super::TakeDamage(damageTaken);
	
	if (isDead)
	{
		attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
		
}
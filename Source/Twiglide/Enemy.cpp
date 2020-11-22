// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	param.Name = "OnHit";
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	material = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, material);
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

	float scalarValue = 0.0f;

	material->GetScalarParameterValue(param, scalarValue);

	if (scalarValue > 0.0f)
	{
		scalarValue -= DeltaTime;
		material->SetScalarParameterValue("OnHit", scalarValue);
	}

	/*if (isInAirCombat)
	{
		airCombatTimer += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "In AirCombat");
		if (airCombatTimer >= airCombatDuration)
		{
			GetCharacterMovement()->GravityScale = 1.0f;
			airCombatTimer = 0.0f;
			isInAirCombat = false;
		}
	}*/
}

void AEnemy::TakeDamage(int damageTaken)
{
	Super::TakeDamage(damageTaken);

	material->SetScalarParameterValue("OnHit", 1.0f);

	/*if (isDead)
		SetActorEnableCollision(false);*/
}
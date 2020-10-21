// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	material = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, material);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canAttack && !isHit)
	{
		attackTimer += DeltaTime;
		if (attackTimer >= attackCooldown)
		{
			attackTimer = 0.f;
			canAttack = false;
		}
	}

	float scalarValue = 0.0f;

	FMaterialParameterInfo param;
	param.Name = "OnHit";

	material->GetScalarParameterValue(param, scalarValue);

	if (scalarValue > 0.0f)
	{
		scalarValue -= DeltaTime;

		material->SetScalarParameterValue("OnHit", scalarValue);
	}
}

void AEnemy::TakeDamage(int damageTaken)
{
	Super::TakeDamage(damageTaken);

	material->SetScalarParameterValue("OnHit", 1.0f);

	if(isDead)
		SetActorEnableCollision(false);
}
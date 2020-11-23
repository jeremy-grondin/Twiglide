// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericCharacter.h"
#include "AttackComponent.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGenericCharacter::AGenericCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	attackBox->SetupAttachment(RootComponent);

	param.Name = "OnHit";
}

void AGenericCharacter::CheckScalarValue(float value,float deltaTime)
{
	if (value > 0.0f)
	{
		value -= deltaTime;
		material->SetScalarParameterValue("OnHit", value);
	}
}

// Called when the game starts or when spawned
void AGenericCharacter::BeginPlay()
{
	Super::BeginPlay();
	attackBox->OnComponentBeginOverlap.AddDynamic(this, &AGenericCharacter::OnOverlap);
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	material = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, material);

	life = maxLife;
}

// Called every frame
void AGenericCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isInAirCombat)
	{
		airCombatTimer += DeltaTime;
		if (airCombatTimer >= airCombatDuration)
		{
			GetCharacterMovement()->GravityScale = 1.0f;
			airCombatTimer = 0.0f;
			isInAirCombat = false;
		}
	}

	float scalarValue = 0.0f;

	material->GetScalarParameterValue(param, scalarValue);

	CheckScalarValue(scalarValue, DeltaTime);

	
}

// Called to bind functionality to input
void AGenericCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGenericCharacter::TakeDamage(int damageTaken)
{
	life -= damageTaken;

	if (life <= 0)
	{
		life = 0;
		isDead = true;
	}

	material->SetScalarParameterValue("OnHit", 1.0f);
}

void AGenericCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent,
				AActor* OtherActor,
				UPrimitiveComponent* OtherComp,
				int32 OtherBodyIndex,
				bool bFromSweep,
				const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Max() != 0
		&& OtherActor->Tags[0] == "Player" && !ActorHasTag("Player"))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

		if (player->isDefending)
		{
			FVector pos = GetOwner()->GetActorLocation() - player->GetActorLocation();
			//block forward attack
			if (FVector::DotProduct(pos, player->GetActorForwardVector()) <= 0)
				player->TakeDamage(damage);
		}
		else
			player->TakeDamage(damage);
		
			
	}
}

void AGenericCharacter::Attack()
{
	if(!isAttacking)
		isAttacking = true;

	damage = lightDamage;

	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGenericCharacter::StopAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	heavyAttack = false;
}

void AGenericCharacter::HeavyAttack()
{
	Attack();

	damage = heavyDamage;
	heavyAttack = true;
}

void AGenericCharacter::freezeMovemnent()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->GravityScale = 0.0f;
}
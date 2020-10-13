// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericCharacter.h"
#include "AttackComponent.h"
#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AGenericCharacter::AGenericCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	attackBox->SetupAttachment(RootComponent);

	life = maxLife;
}

// Called when the game starts or when spawned
void AGenericCharacter::BeginPlay()
{
	Super::BeginPlay();
	attackBox->OnComponentBeginOverlap.AddDynamic(this, &AGenericCharacter::OnOverlap);
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AGenericCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}

void AGenericCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent,
				AActor* OtherActor,
				UPrimitiveComponent* OtherComp,
				int32 OtherBodyIndex,
				bool bFromSweep,
				const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Max() != 0
		&& OtherActor->Tags[0] == "Enemy" && !ActorHasTag("Enemy"))
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);

		if (!enemy->isDead)
			enemy->TakeDamage(damage);
	}
	else if (OtherActor->Tags.Max() != 0
		&& OtherActor->Tags[0] == "Player" && !ActorHasTag("Player"))
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		if(!player->isDead)
			player->TakeDamage(damage);
	}
}

void AGenericCharacter::Attack()
{
	if(!isAttacking)
		isAttacking = true;

	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AGenericCharacter::StopAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
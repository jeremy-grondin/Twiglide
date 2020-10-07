// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "Components/BoxComponent.h"
#include "Collision.h"
#include "Enemy.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bCanEverTick = true;
	
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollider"));
	box->SetupAttachment(this);

}

// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	if(box)
		box->OnComponentBeginOverlap.AddDynamic(this, &UAttackComponent::OnOverlap);
}

// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackComponent::OnOverlap(UPrimitiveComponent* OverlappedComponent,
								AActor* OtherActor,
								UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex,
								bool bFromSweep,
								const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "HIT");
	if (OtherActor->Tags.Max() != 0
		&& OtherActor->Tags[0] == "Enemy")
	{
		AEnemy* enemy = Cast<AEnemy>(OtherActor);

		if (!enemy->isDead)
			enemy->TakeDamage(damage);
	}
}
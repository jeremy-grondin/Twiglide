// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AttackComponent.generated.h"

enum EAttackType
{
	Light,
	Heavy
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWIGLIDE_API UAttackComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	EAttackType attackType;

	class UBoxComponent* box;

public:	
	// Sets default values for this component's properties
	UAttackComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackStats)
	int damage = 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
					AActor* OtherActor,
					UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex,
					bool bFromSweep,
					const FHitResult& SweepResult);
};

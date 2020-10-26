// Fill out your copyright notice in the Description page of Project Settings.


#include "StartCombo.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UStartCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (player != nullptr)
	{
		player->StartCombo();
		player->GetWorldTimerManager().SetTimer(player->timerHandler, player, &APlayerCharacter::StopCombo, player->comboTime, false);
	}
}
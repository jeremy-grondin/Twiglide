// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Twiglide_Instance.generated.h"

/**
 * 
 */
UCLASS()
class TWIGLIDE_API UTwiglide_Instance : public UGameInstance
{
	GENERATED_BODY()

public :

	UPROPERTY(BlueprintReadWrite)
	bool bInverseXAxis;

	UPROPERTY(BlueprintReadWrite)
	bool bInverseYAxis;
	
};

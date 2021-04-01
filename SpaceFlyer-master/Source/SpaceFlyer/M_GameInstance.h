// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "M_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFLYER_API UM_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UM_GameInstance();

	bool isSequenceFinished;
	
};

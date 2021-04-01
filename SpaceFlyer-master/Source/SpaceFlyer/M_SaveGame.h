// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "M_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFLYER_API UM_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UM_SaveGame();
	
	bool isSequenceFinished;
	
};

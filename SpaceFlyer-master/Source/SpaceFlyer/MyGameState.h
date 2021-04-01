// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFLYER_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()

		AMyGameState();

public:
	class AMyPlayer* m_Player;

	UFUNCTION(BlueprintPure, Category = "Time")
		int GetGameSeconds();
	UFUNCTION(BlueprintPure, Category = "Time")
		int GetGameMinutes();

	void UpdateSeconds();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
	void SetGamePaused(bool bIsPaused);
	bool isGamePaused();

	int Seconds;
	int Minutes;
	bool isPaused;

	float startTime;
	bool TimeStart;
};

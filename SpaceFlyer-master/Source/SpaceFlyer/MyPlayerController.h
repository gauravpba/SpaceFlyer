// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFLYER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = CustomProperties) 
		class AMyPlayer* MyPlayer;
	
	UStaticMeshComponent* FoundComp;
	class AM_GameMode* m_GameMode;
	class AMyGameState* m_GameState;

	void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveXAxis(float value);
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveYAxis(float value);

	void Jump();
	void Pause();

	FVector CurrentVelocity;
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere)
		float JumpForce;
};

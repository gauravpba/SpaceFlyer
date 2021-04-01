// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "M_GameMode.h"
#include "MyGameState.h"
#include "EngineUtils.h"


void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//MyPlayer = Cast<AMyPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	m_GameMode = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	m_GameState = Cast<AMyGameState>(GetWorld()->GetGameState());

	//MyPlayer = Cast<AMyPlayer>(m_GameMode->DefaultPawnClass.GetDefaultObject());

	for (TActorIterator<AMyPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		MyPlayer = *ActorItr;
	}

	
	this->InputComponent->BindAxis("MoveX", this, &AMyPlayerController::MoveXAxis);
	this->InputComponent->BindAxis("MoveY", this, &AMyPlayerController::MoveYAxis);
	
	
	
	this->InputComponent->BindAction("Pause", IE_Pressed,this, &AMyPlayerController::Pause).bExecuteWhenPaused = true;
	
	MoveSpeed = 100.0f;
	JumpForce = 500.0f;
}


void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetPawn()))
		{
			if (MyPlayer->bGameEnded == false && MyPlayer->isSequenceFinished == true)
			{
				if (!CurrentVelocity.IsZero())
				{
					FVector NewLocation = MyPlayer->GetActorLocation() + (CurrentVelocity * DeltaTime);
					MyPlayer->SetActorLocation(NewLocation);
				}
			}
		}
	}
}

void AMyPlayerController::MoveXAxis(float Value)
{	
	if (m_GameMode->GetCurrentState() != EGamePlayState::EGameOver)
	{
		CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * MoveSpeed;
		if (MoveSpeed < 1500.0f)
			MoveSpeed += 100.0f;
	}
}

void AMyPlayerController::MoveYAxis(float Value)
{
	if (m_GameMode->GetCurrentState() != EGamePlayState::EGameOver)
		CurrentVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * 1000.0f;

}

void AMyPlayerController::Jump()
{
	MyPlayer->isSequenceFinished = true;
	MyPlayer->Jump();
}

void AMyPlayerController::Pause()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, m_GameState->isGamePaused() ? "True":"False"); // x is the variable name

	if (m_GameState->isGamePaused() == false)
	{
		m_GameState->SetGamePaused(true);
	}
	else
		m_GameState->SetGamePaused(false);
}
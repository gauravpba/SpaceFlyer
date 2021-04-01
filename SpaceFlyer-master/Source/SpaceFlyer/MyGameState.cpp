// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"
#include "MyPlayerController.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "MyPlayer.h"
#include "EngineUtils.h"

AMyGameState::AMyGameState()
{
	Seconds = 0;
	Minutes = 0;
	isPaused = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	TimeStart = false;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AMyPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		m_Player = *ActorItr;
	}
}

int AMyGameState::GetGameSeconds()
{
	return Seconds;
}

int AMyGameState::GetGameMinutes()
{
	return Minutes;
}



void AMyGameState::UpdateSeconds()
{
	
	float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld())- startTime;
	Seconds = fmod(realtimeSeconds, 60);
	Minutes = FMath::FloorToInt(realtimeSeconds / 60);
	

}

void AMyGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!isPaused && m_Player->isSequenceFinished == true)
	{
		UpdateSeconds();
		if (TimeStart == false) 
		{
			startTime = GetGameTimeSinceCreation();
			TimeStart = true;
		}
	}
}

void AMyGameState::SetGamePaused(bool bIsPaused)
{
	AMyPlayerController* const MyPlayer = Cast<AMyPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	
	if (MyPlayer != NULL )
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, bIsPaused ? "True" : "False");

		isPaused = bIsPaused;
		MyPlayer->SetPause(bIsPaused);
		
		

	}
}

bool AMyGameState::isGamePaused()
{
	return isPaused;
}

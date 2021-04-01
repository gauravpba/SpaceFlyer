// Fill out your copyright notice in the Description page of Project Settings.

#include "M_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/GameEngine.h"
#include "MyPlayer.h"
#include "ConstructorHelpers.h"
#include "Engine/ObjectLibrary.h"
#include "MyGameState.h"
#include "EngineUtils.h"
#include "M_GameInstance.h"

AM_GameMode::AM_GameMode()
{
	Levels = GetAllMapNames();
	
}

void AM_GameMode::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputMode;

	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = false;

	SetCurrentState(EGamePlayState::EPlaying);

	for (TActorIterator<AMyPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr) { 
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		myPlayer = *ActorItr; 
	} 
	
	FString LevelName = GetWorld()->GetMapName();
	TArray<FString> Out;
	LevelName.ParseIntoArray(Out, TEXT("_"), true);
	FString LevelNumber = Out[Out.Num() - 1];
	currentLevelIndex = FCString::Atof(*LevelNumber);
	
	m_GameState = Cast<AMyGameState>(GetWorld()->GetGameState());
	isGameOver = false;

	if (currentLevelIndex == 3)
		myPlayer->isSequenceFinished = true;
	else
	{
		
		UGameInstance* gameInst = GetWorld()->GetGameInstance();
		m_GameInstance = Cast<UM_GameInstance>(gameInst);
		if (m_GameInstance != nullptr) 
		{
			myPlayer->isSequenceFinished = m_GameInstance->isSequenceFinished;
		}
		else
			myPlayer->isSequenceFinished = false;

	}
	
}


TArray<FString> AM_GameMode::GetAllMapNames() {
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/StarterContent/SpaceFlyer/Levels"));
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	UE_LOG(LogTemp, Warning, TEXT("Found maps: %d"), AssetDatas.Num());

	TArray<FString> Names = TArray<FString>();

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];

		auto name = AssetData.AssetName.ToString();
		Names.Add(name);
	}
	return Names;
}

void AM_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (myPlayer)
	{
		if (FMath::IsNearlyZero(myPlayer->GetGas(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}
	}
}

EGamePlayState AM_GameMode::GetCurrentState() const
{
	return EGamePlayState();
}

void AM_GameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}


void AM_GameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
		case EGamePlayState::EPlaying:
		{
			break;
		}
		case EGamePlayState::EGameOver:
		{
			EndGame();
		}
		
		case EGamePlayState::EUnknown:
		{
			break;
		}
	}
}


void AM_GameMode::CheckLevel()
{
	if (currentLevelIndex < Levels.Num() - 1)
	{
		currentLevelIndex += 1;
		NextLevel = Levels[currentLevelIndex];
	}
	else
	{
		NextLevel = "End";
	}
}

void AM_GameMode::EndGame()
{
	
	m_GameState->Reset();
	//GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &AM_GameMode::ReloadLevel, 2.0f, false);
}

bool AM_GameMode::PlayDeadAnim()
{
	if (DeadFlash)
	{
		DeadFlash = false;
		return true;
	}
	return false;
}

bool AM_GameMode::PlayLevelClearAnim()
{
	if (LevelCleared)
	{
		LevelCleared = false;
		return true;
	}
	return false;
}

bool AM_GameMode::PlayEmptyGasAnim()
{
	if (EmptyGas)
	{
		EmptyGas = false;
		return true;
	}
	return false;
}

void AM_GameMode::LevelCompleted()
{
	m_GameInstance->isSequenceFinished = false;
}

void AM_GameMode::ReloadLevel()
{

	UGameplayStatics::OpenLevel(this, FName(*Levels[currentLevelIndex]), false);

}

void AM_GameMode::ChangeLevel()
{
	CheckLevel();

	if (NextLevel != "End")
	{
		UGameplayStatics::OpenLevel(this, FName(*NextLevel), false);
	}
	else
		ReturnToMainMenu();
}

void AM_GameMode::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(this, FName(*Levels[0]), false);
}




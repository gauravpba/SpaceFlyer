// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "M_GameMode.generated.h"


UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS()
class SPACEFLYER_API AM_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AM_GameMode();

	void EndGame();
	void LevelCompleted();
	
	UFUNCTION(BlueprintCallable, Category = "LevelChange")
		void ReloadLevel();
	
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	class AMyPlayer* myPlayer;

	class AMyGameState* m_GameState;
	class UM_GameInstance* m_GameInstance;

	UFUNCTION(BlueprintPure, Category = "Gas")
		EGamePlayState GetCurrentState() const;

	
	void SetCurrentState(EGamePlayState NewState);
	
	UFUNCTION(BlueprintCallable,Category = "LevelChange")
		void ChangeLevel();

	UFUNCTION(BlueprintCallable, Category = "LevelChange")
		void ReturnToMainMenu();

	bool isGameOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
		bool DeadFlash;

	UFUNCTION(BlueprintPure, Category = "Death")
		bool PlayDeadAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
		bool LevelCleared;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
		bool LevelFinished;

	UFUNCTION(BlueprintPure, Category = "Death")
		bool PlayLevelClearAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
		bool EmptyGas;
	UFUNCTION(BlueprintPure, Category = "Death")
		bool PlayEmptyGasAnim();

	int32 currentLevelIndex;

protected:
	FTimerHandle LevelSwapTimer;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Levels")
		TArray<FString> Levels;

	APlayerController* Controller;
	FString NextLevel;
	void CheckLevel();
	TArray<FString> GetAllMapNames();
	EGamePlayState CurrentState;
	void HandleNewState(EGamePlayState NewState);

};

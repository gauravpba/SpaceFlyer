// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayer.generated.h"


UCLASS()
class SPACEFLYER_API AMyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	//vars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		UStaticMeshComponent* ourVisibleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
		class UCameraComponent* ourCamera;

	UStaticMeshComponent* FoundComp;
	class AM_GameMode* m_GameMode;
	class AMyGameState* m_GameState;
	class AMyPlayerController* m_Controller;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gas")
		float fullGas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gas")
		float Gas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gas")
		float gasPercentage;


	UPROPERTY(EditAnywhere, Category = "Gas")
		FTimerHandle MemberTimerHandle;


	float TimelineValue;
	bool bCanBurnFuel;

	//Function
	UFUNCTION(BlueprintPure, Category = "Gas")
		float GetGas();

	UFUNCTION(BlueprintPure, Category = "Gas")
		FText GetGasIntText();

	UFUNCTION()
		void GasTimer();

	UFUNCTION()
		void setGasState();

 
	UFUNCTION(BlueprintCallable, Category = "Gas")
		void UpdateGas(float GasChange);

	void Jump();

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		class USoundCue* CrashAudioCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		class UAudioComponent* CrashAudioComponent;


	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		class USoundCue* FuelUpCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		class UAudioComponent* FuelUpComponent;
	
	UFUNCTION(BlueprintCallable, Category = "Death")
		void GameEnded();

	UPROPERTY(EditAnywhere)
		class UParticleSystem* ExplosionPS;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* SmokePS;


	UPROPERTY(EditAnywhere)
		class UParticleSystem* FuelUpPS;

	bool bGameEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
		bool isSequenceFinished;

	class UM_GameInstance* m_GameInstance;

	UFUNCTION(BlueprintCallable, Category = "Game_Instance")
		void setInstance();


protected:
	//vars
	FTimerHandle EndGameTimer;
	FVector currentVelocity;
	float MoveSpeed;
	FVector Start;
	FVector End;
	bool isJumping;
	UPROPERTY(VisibleAnywhere)
		float JumpForce;

	//Functions


	void EndGame();
	void EmptyGas();
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
			FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
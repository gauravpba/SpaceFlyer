// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/StaticMesh.h"
#include "M_Obstacles.h"
#include "M_EndPoint.h"
#include "M_GameMode.h"
#include "MyPlayerController.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyGameState.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "M_GameInstance.h"


// Sets default values
AMyPlayer::AMyPlayer()
{
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Flying/Meshes/UFO"));

	ourVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = ourVisibleComponent;
	if (Mesh != NULL)
	{
		ourVisibleComponent->SetStaticMesh(Mesh);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(ourVisibleComponent);

	ourCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ourCamera->SetupAttachment(SpringArm);

	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	TArray<UStaticMeshComponent*> Comps;
	GetComponents(Comps);
	if (Comps.Num() > 0)
	{
		FoundComp = Comps[0];
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> CrashCue(TEXT("SoundCue'/Game/StarterContent/SpaceFlyer/Audio/Crash.Crash'"));

	// Store a reference to the Cue asset - we'll need it later.
	CrashAudioCue = CrashCue.Object;
	// Create an audio component, the audio component wraps the Cue, and allows us to ineract with
	// it, and its parameters from code.
	CrashAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CrashAudioComp"));
	// I don't want the sound playing the moment it's created.
	CrashAudioComponent->bAutoActivate = false; // don't play the sound immediately.
	// I want the sound to follow the pawn around, so I attach it to the Pawns root.
	CrashAudioComponent->SetupAttachment(ourVisibleComponent);
	// I want the sound to come from slighty in front of the pawn.
	CrashAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USoundCue> GasCue(TEXT("SoundCue'/Game/StarterContent/SpaceFlyer/Audio/FuelCanCue.FuelCanCue'"));

	FuelUpCue = GasCue.Object;
	FuelUpComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FuelAudioComp"));
	FuelUpComponent->bAutoActivate = false; // don't play the sound immediately.
	FuelUpComponent->SetupAttachment(ourVisibleComponent);
	FuelUpComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/StarterContent/SpaceFlyer/ParticleSystem/Player_Explosion.Player_Explosion'"));
	ExplosionPS = Particle.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FuelPart(TEXT("ParticleSystem'/Game/StarterContent/SpaceFlyer/ParticleSystem/FuelUpPS.FuelUpPS'"));
	FuelUpPS = FuelPart.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeParticle(TEXT("ParticleSystem'/Game/StarterContent/SpaceFlyer/ParticleSystem/P_Smoke.P_Smoke'"));
	SmokePS = SmokeParticle.Object;

	fullGas = 1000.0f;
	Gas = fullGas;
	gasPercentage = 1.0f;
	bGameEnded = false;

	JumpForce = 500.0f;


	isSequenceFinished = false;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	ourVisibleComponent->OnComponentHit.AddDynamic(this, &AMyPlayer::OnHit);
	ourVisibleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnBeginOverlap);
	m_GameMode = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
	m_GameState = Cast<AMyGameState>(GetWorld()->GetGameState());
	m_GameInstance = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
	bGameEnded = false;
	m_GameMode->EmptyGas = false;
	bCanBurnFuel = true;
	ourVisibleComponent->SetSimulatePhysics(true);

}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CrashAudioCue->IsValidLowLevelFast()) {
		CrashAudioComponent->SetSound(CrashAudioCue);
	}
	if (FuelUpCue->IsValidLowLevelFast()) {
		FuelUpComponent->SetSound(FuelUpCue);
	}


}
// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_GameMode->GetCurrentState() == EGamePlayState::EPlaying && isSequenceFinished == true)
	{
		Start = GetActorLocation();
		End = Start + (GetActorUpVector() * -200.0f);

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		//Falls off the platform
		if (!RV_Hit.bBlockingHit && isJumping == false && this->GetActorLocation().Z < -20.0f && m_GameMode->LevelCleared != true)
		{
			GameEnded();
		}
		if (bGameEnded == false && m_GameMode->LevelFinished != true) {
			UpdateGas(-1.5f);
			GasTimer();
		}
	}

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::Jump);

}


void AMyPlayer::OnHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{

	if (Other->ActorHasTag("Obstacle") || Other->ActorHasTag("RedPlatform"))
	{	
		GameEnded();
	}
	else if (Other->ActorHasTag("Platform"))
	{
		isJumping = false;
	}
}

void AMyPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("EndPoint"))
	{
		AM_EndPoint* LevelEnd = Cast<AM_EndPoint>(OtherActor);

		if (LevelEnd)
		{
			m_GameMode->LevelCleared = true;
			m_GameMode->LevelFinished = true;
			m_GameMode->LevelCompleted();

		}
	}
}

void AMyPlayer::GameEnded()
{
	if (bGameEnded)
	{
		return;
	}
	bGameEnded = true;
	CrashAudioComponent->Play();
	ourVisibleComponent->SetSimulatePhysics(false);
	this->SetActorHiddenInGame(true);
	m_GameMode->DeadFlash = true;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionPS, GetActorTransform(),true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokePS, GetActorTransform(), true);
	GetWorldTimerManager().SetTimer(EndGameTimer, this, &AMyPlayer::EndGame, 0.1f, false);
}

void AMyPlayer::setInstance()
{
	m_GameInstance->isSequenceFinished = isSequenceFinished;
}

void AMyPlayer::EndGame()
{
	if(m_GameMode)
		m_GameMode->SetCurrentState(EGamePlayState::EGameOver);
}

void AMyPlayer::EmptyGas()
{
	m_GameMode->EmptyGas = true;
	bGameEnded = true;
	GetWorldTimerManager().SetTimer(EndGameTimer, this, &AMyPlayer::EndGame, 0.1f, false);

}

void AMyPlayer::Jump()
{
	if (isJumping == false && isSequenceFinished == true)
	{
		FoundComp->AddImpulse(GetActorUpVector() * JumpForce, "None", true);
		isJumping = true;
	}
}

float AMyPlayer::GetGas()
{
	return gasPercentage;
}

FText AMyPlayer::GetGasIntText()
{
	int32 GP = FMath::RoundHalfFromZero(gasPercentage * 100);
	FString GPS = FString::FromInt(GP);
	FString GasHUD = GPS + FString(TEXT("%"));
	FText GasText = FText::FromString(GasHUD);

	return GasText;
}

void AMyPlayer::setGasState()
{
	if(!bGameEnded)
		bCanBurnFuel = true;

}

void AMyPlayer::GasTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AMyPlayer::setGasState, 2.0f, false);
}



void AMyPlayer::UpdateGas(float GasChange)
{
	if (GasChange > 0)
	{
		FuelUpComponent->Play();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FuelUpPS, GetActorTransform(),true);

	}
	Gas += GasChange;
	Gas = FMath::Clamp(Gas, 0.0f, fullGas);
	gasPercentage = Gas / fullGas;

	if (gasPercentage == 0 && bCanBurnFuel == true)
	{
		EmptyGas();
		bCanBurnFuel = false;

	}
}
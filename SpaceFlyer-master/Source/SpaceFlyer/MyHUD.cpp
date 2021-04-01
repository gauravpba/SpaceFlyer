// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "M_GameMode.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> GasBarObj(TEXT("/Game/StarterContent/SpaceFlyer/UI/HUDUI"));

	HUDWidgetClass = GasBarObj.Class;
	
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
	m_GameMode = Cast<AM_GameMode>(GetWorld()->GetAuthGameMode());
}

void AMyHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (m_GameMode->isGameOver)
	{
		Reset();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFLYER_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AMyHUD(); 
	
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gas")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Gas")
		class UUserWidget* CurrentWidget;

	class AM_GameMode* m_GameMode;

	
	

};

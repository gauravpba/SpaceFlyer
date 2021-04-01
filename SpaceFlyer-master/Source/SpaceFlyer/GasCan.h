// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasCan.generated.h"


UCLASS()
class SPACEFLYER_API AGasCan : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasCan();

public:
	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		class AMyPlayer* MyPlayer;



};

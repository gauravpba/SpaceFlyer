// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "M_Obstacles.generated.h"

UCLASS()
class SPACEFLYER_API AM_Obstacles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AM_Obstacles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Called every frame
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Setup")
		UStaticMeshComponent* cube;
	
	bool bImGoingUp = true;
	FVector StartLocation;
	
};

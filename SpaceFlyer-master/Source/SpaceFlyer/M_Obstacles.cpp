// Fill out your copyright notice in the Description page of Project Settings.

#include "M_Obstacles.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AM_Obstacles::AM_Obstacles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = cube;

	StartLocation = GetActorLocation();
}

// Called when the game starts or when spawned
void AM_Obstacles::BeginPlay()
{
	Super::BeginPlay();
	
}


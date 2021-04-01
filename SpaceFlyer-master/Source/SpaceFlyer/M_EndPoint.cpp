// Fill out your copyright notice in the Description page of Project Settings.

#include "M_EndPoint.h"
#include "Components/BoxComponent.h"


// Sets default values
AM_EndPoint::AM_EndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;


}

// Called when the game starts or when spawned
void AM_EndPoint::BeginPlay()
{
	Super::BeginPlay();
	
}




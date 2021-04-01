// Fill out your copyright notice in the Description page of Project Settings.

#include "GasCan.h"
#include "MyPlayer.h"

// Sets default values
AGasCan::AGasCan()
{
	OnActorBeginOverlap.AddDynamic(this,&AGasCan::OnOverlap);

}


void AGasCan::OnOverlap(AActor * MyOverlappedActor, AActor * OtherActor)
{
	if (OtherActor != nullptr && OtherActor != this )
	{
		MyPlayer = Cast<AMyPlayer>(OtherActor);
		if (MyPlayer && MyPlayer->GetGas() < 1.0f)
		{
			MyPlayer->UpdateGas(250.0f);
			Destroy();
		}
	}
}

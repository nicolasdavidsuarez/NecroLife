// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Public/ItemXP.h"
#include "Components/Public/RPGHelper.h"


// Sets default values
AItemXP::AItemXP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AItemXP::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (URPGHelper::TakeXP(OtherActor,13))
	{
		Destroy();	
	}
		
}

// Called when the game starts or when spawned
void AItemXP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemXP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


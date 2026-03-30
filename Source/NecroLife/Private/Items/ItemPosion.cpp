// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Items/ItemPosion.h"

#include "Public/Components/RPGHelper.h"


// Sets default values
AItemPosion::AItemPosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemPosion::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemPosion::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (URPGHelper::TakePosion(OtherActor))
	{
		Destroy();	
	}
		
}
// Called every frame
void AItemPosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


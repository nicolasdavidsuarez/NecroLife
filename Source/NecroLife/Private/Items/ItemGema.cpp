// Fill out your copyright notice in the Description page of Project Settings.


#include "Items\ItemGema.h"


// Sets default values
AItemGema::AItemGema()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemGema::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemGema::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UInventoryComponent* Inventory= OtherActor->FindComponentByClass<UInventoryComponent>();
	
	Inventory->AddGems(GemaData);
	Destroy();
}

// Called every frame
void AItemGema::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


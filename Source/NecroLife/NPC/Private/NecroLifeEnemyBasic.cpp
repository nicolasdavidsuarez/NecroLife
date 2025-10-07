// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Public/NecroLifeEnemyBasic.h"




// Sets default values
ANecroLifeEnemyBasic::ANecroLifeEnemyBasic()
{
  //////	componente de salud
	HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ANecroLifeEnemyBasic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANecroLifeEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANecroLifeEnemyBasic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


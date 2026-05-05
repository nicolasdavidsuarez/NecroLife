// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/NPC/NecroLifeEnemyBasic.h"




// Sets default values
ANecroLifeEnemyBasic::ANecroLifeEnemyBasic()
{
  //////	componente de salud
	HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(RootComponent);
	TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
	TargetWidget->SetDrawSize(FVector2D(64.0f, 64.0f));
	TargetWidget->SetVisibility(false); // Arranca invisible
	
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

bool ANecroLifeEnemyBasic::IsAlive()
{
	if (HealthComponent->IsDead())
	{
		return false;
	}
	return true;
}


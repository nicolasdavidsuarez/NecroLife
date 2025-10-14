// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/MovingPlatform.h"


// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	RootComponent = PlatformMesh;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Current = GetActorLocation();
	FVector Target = bGoingForward ? EndPoint : StartPoint;

	FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
	SetActorLocation(NewLocation);

	if (FVector::Dist(NewLocation, Target) < 2.f)
	{
		bGoingForward = !bGoingForward;
	}
}


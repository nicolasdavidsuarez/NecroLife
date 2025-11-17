// Fill out your copyright notice in the Description page of Project Settings.


#include "Scene/AdvancedPlatform.h"

#include "IAutomationControllerManager.h"


// Sets default values
AAdvancedPlatform::AAdvancedPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	PlatformMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	PlatformDoors=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Doors"));
	RootComponent=PlatformMesh;
	PlatformDoors->SetupAttachment(PlatformMesh);
	SphereCollision->SetupAttachment(PlatformMesh);
	
}

// Called when the game starts or when spawned
void AAdvancedPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AAdvancedPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Path.Num()>0&&bIsOverlappingPlatform)
	{
		FVector Current = GetActorLocation();
		FVector Target = Path[Nodo];

		FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, Target) < 2.f)
		{
	     Nodo++;
			if (Path.Num()==Nodo)
			{
				Destroy();
			}
		}
	}
	
}

void AAdvancedPlatform::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	bIsOverlappingPlatform=true;
	PlatformDoors->SetRelativeLocation(PlatformClose);
	
}


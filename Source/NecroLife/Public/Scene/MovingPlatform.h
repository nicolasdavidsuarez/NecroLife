// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class NECROLIFE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector StartPoint;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector EndPoint;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;

private:
	bool bGoingForward = true;
};

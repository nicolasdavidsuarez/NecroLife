// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NecroLifeCharacter.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class NECROLIFE_API AInteractableBase : public AActor, public INecroLifeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, ...);
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, ...);
	void RequestCharacterLookAt_Implementation(ANecroLifeCharacter* CharacterToRotate);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractableBase.h"

#include "NecroLifeCharacter.h"


// Sets default values
AInteractableBase::AInteractableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, ...)
{
	ANecroLifeCharacter* PJ = Cast<ANecroLifeCharacter>(OtherActor);
	if (PJ)
	{
		// Le pasamos "this" (este objeto) al personaje
		PJ->SetCurrentInteractable(this);
	}
}

void AInteractableBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, ...)
{
	ANecroLifeCharacter* PJ = Cast<ANecroLifeCharacter>(OtherActor);
	if (PJ)
	{
		// Limpiamos la referencia cuando el PJ se aleja
		PJ->ClearCurrentInteractable();
	}
	
}

void AInteractableBase::RequestCharacterLookAt_Implementation(ANecroLifeCharacter* CharacterToRotate)
{
	// Aunque esté vacío por ahora
}
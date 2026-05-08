// Fill out your copyright notice in the Description page of Project Settings.


#include "Items\ItemGema.h"

#include "NecroLifeCharacter.h"
#include "NecroLifePlayerState.h"


// Sets default values
AItemGema::AItemGema()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	bReplicates=true;
}

// Called when the game starts or when spawned
void AItemGema::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemGema::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority()) 
	{
		return; 
	}
	
	APawn* Pawn=Cast<APawn>(OtherActor);
	APlayerState* PS = Pawn->GetPlayerState();

	if (PS)
	{
			UInventoryComponent* Inventory= PS->FindComponentByClass<UInventoryComponent>();
		if (Inventory)
		{
			Inventory->AddGems(GemaData);
			ANecroLifeCharacter* character=Cast<ANecroLifeCharacter>(Pawn);
			if (character)
			{
				character->MostarNuevaGema(GemaData);
			}
			
			
			Destroy();   
		}
			
	}

}

// Called every frame
void AItemGema::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


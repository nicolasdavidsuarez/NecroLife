// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NecroLifeForgeNpc.h"

#include "NecroLifeCharacter.h"


// Sets default values
ANecroLifeForgeNpc::ANecroLifeForgeNpc()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ANecroLifeForgeNpc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANecroLifeForgeNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANecroLifeForgeNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANecroLifeForgeNpc::OnInteract_Implementation(AActor* Interactor)
{
	Super::OnInteract_Implementation(Interactor);
	ANecroLifeCharacter* myCharacter = Cast<ANecroLifeCharacter>(Interactor);
	if (myCharacter)
	{
		myCharacter->ShowForgeInventoryAction();
	}
}

TMap<FName, int32> ANecroLifeForgeNpc::GetPossibleUpgrades(const TArray<FDatosGema>& GemsItems)
{
	// Inventario actual de cuantas gemas de cada tipo tengo
	TMap<FName, int32> InventarioActual;
    
	// inventario de cuantas se pueden crear
	TMap<FName, int32> UpgradesDisponibles;

	// Llenamos el conteo del inventario actual
	for (const FDatosGema& Gema : GemsItems)
	{
		if (!Gema.ID_Gema.IsNone())
		{
			InventarioActual.FindOrAdd(Gema.ID_Gema)++;
		}
	}

	// vemos cuales se pueden procesar
	// Solo necesitamos iterar sobre el inventario una vez
	for (const FDatosGema& Gema : GemsItems)
	{
		// Solo verificamos si la gema tiene una evolución (Puede que no se pueda)
		//
		if (!Gema.ID_NextGema.IsNone() && !UpgradesDisponibles.Contains(Gema.ID_NextGema))
		{
			int32 CantidadQueTiene = InventarioActual.FindRef(Gema.ID_Gema);
			if (Gema.NeedNextGema > 0 && CantidadQueTiene >= Gema.NeedNextGema)
			{
				int32 CantidadACrear = CantidadQueTiene / Gema.NeedNextGema;
            	UpgradesDisponibles.Add(Gema.ID_NextGema, CantidadACrear);
			}
		}
	}
	return UpgradesDisponibles;
}

void ANecroLifeForgeNpc::ForgeGems(const TArray<FGemsItems>& GemsItems)
{
	if (HasAuthority()&&GemsItems.Num()>0)
    	{
    		
    	}else
    	{
    		Server_ForgeGems(GemsItems);
    	}
}




void ANecroLifeForgeNpc::Server_ForgeGems_Implementation(const TArray<FGemsItems>& GemsItems)
{
	ForgeGems(GemsItems);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InventoryComponent.h"

#include "Components/AttributeComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::TakeHealthPosion(int Amount)
{
	HealthPosion += Amount;
	OnPosionChange.Broadcast(HealthPosion);
}

bool UInventoryComponent::UseHealtPosion()
{
	if (HealthPosion > 0)
	{
		HealthPosion -= 1;
		OnPosionChange.Broadcast(HealthPosion);
     		return true;
	}else{
		return false;
	}
	
}


void UInventoryComponent::PickUp(UItemData* Aitem)
{
	if (UItemData* Item=Cast<UItemData>(Aitem))
	{
		/*FString ItemName = Item->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ItemName);*/
		InventoryItems.Add(Item);
	}
	OnShowItem.Broadcast(InventoryItems);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddGems(FDatosGema gema)
{
	GemsItems.Add(gema);
	GemsToShow.Broadcast(GemsItems);
}

void UInventoryComponent::AddGemToSlot(FDatosGema gema)
{
	// 1. Buscamos la gema en el inventario principal
	for (int32 i = 0; i < GemsItems.Num(); ++i)
	{
		// Comparamos por el ID para saber que es la gema correcta
		if (GemsItems[i].ID_Gema == gema.ID_Gema)
		{
			// La borramos del inventario
			GemsItems.RemoveAt(i);
            
			// Usamos break para que, si tenés 3 gemas iguales, solo borre UNA
			break; 
		}
	}
	GemsInSlots.Add(gema);
	GemsToShow.Broadcast(GemsItems);
	if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		Atributos->RecalcularEstadisticas(GemsInSlots);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/AttributeComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(UInventoryComponent, GemsItems);
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

void UInventoryComponent::OnRep_GemsItems()
{
	if (GemsToShow.IsBound())
	{
		GemsToShow.Broadcast(GemsItems);
	}
	
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
	for (int32 i = 0; i < GemsItems.Num(); ++i)
	{
		if (GemsItems[i].ID_Gema == gema.ID_Gema)
		{
			GemsItems.RemoveAt(i);
            
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


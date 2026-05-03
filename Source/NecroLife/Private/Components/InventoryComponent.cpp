// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/AttributeComponent.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	// ...
}


void UInventoryComponent::TakeHealthPosion(int Amount)
{
	HealthPosion += Amount;
	OnPotionChange.Broadcast(HealthPosion);
}

bool UInventoryComponent::UseHealtPosion()
{
	if (HealthPosion > 0)
	{
		HealthPosion -= 1;
		OnPotionChange.Broadcast(HealthPosion);
     		return true;
	}else{
		return false;
	}

}


void UInventoryComponent::PickUp(UItemData* Aitem)
{
	if (UItemData* Item=Cast<UItemData>(Aitem))
	{
		InventoryItems.Add(Item);
	}
	OnShowItem.Broadcast(InventoryItems);
}

void UInventoryComponent::OnRep_GemsItems()
{	
		GemsToShow.Broadcast(GemsItems);
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,"Broadcasted Gems item");
}

void UInventoryComponent::OnRep_GemsInSlots()
{
	GemsToShowInSlots.Broadcast(GemsInSlots);
	GemsToShow.Broadcast(GemsItems);
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,"Broadcasted GemsInSlots");
}

void UInventoryComponent::Server_addGemas_Implementation(FDatosGema gema)
{
	AddGems(gema);
}

void UInventoryComponent::Server_addGemasInSlot_Implementation(FDatosGema gema)
{
	AddGemToSlot(gema);
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

void UInventoryComponent::RequestAddGemToSlot(FDatosGema gema)
{
	// Le pedimos al servidor que lo haga
	Server_AddGemToSlot(gema);
}

void UInventoryComponent::Server_AddGemToSlot_Implementation(FDatosGema gema)
{
		AddGemToSlot(gema); 
}

void UInventoryComponent::AddGems(FDatosGema gema)
{
	if (GetOwner()->HasAuthority())
	{
		GemsItems.Add(gema);
		GemsNotDuplicates(GemsItems, GemsItemsInInventory);
		GemsToShow.Broadcast(GemsItemsInInventory);	
   }else
   {
	   Server_addGemas(gema);
   }
}



void UInventoryComponent::AddGemToSlot(FDatosGema gema)
{
	if (GetOwner()->HasAuthority())
	{
		for (int32 i = 0; i < GemsItems.Num(); ++i)
        	{
        		if (GemsItems[i].ID_Gema == gema.ID_Gema)
        		{
        			FName id=GemsItems[i].ID_Gema;
        			GemsItems.RemoveAt(i);
        			GemsNotDuplicates(GemsItems, GemsItemsInInventory);
        			GemsItemsInInventory.RemoveAll([id](const FDatosGema& Gema){	return Gema.ID_Gema == id;});
        			GemsToShow.Broadcast(GemsItemsInInventory);
        			//break; 
        		}
        	}
        	GemsInSlots.Add(gema);
        	GemsToShowInSlots.Broadcast(GemsInSlots);
	}else
	{
		Server_AddGemToSlot(gema);
	}
	
	
	if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		Atributos->RecalcularEstadisticas(GemsInSlots);
	}
}

void UInventoryComponent::GemsNotDuplicates(const TArray<FDatosGema>& SourceArray, TArray<FDatosGema>& TargetArray)
{
	TargetArray.Empty();
	
	TSet<FName> FnameGema;
	for (const FDatosGema& Gema : SourceArray)
	{
		if (!FnameGema.Contains(Gema.ID_Gema))
		{
			FnameGema.Add(Gema.ID_Gema);
			TargetArray.Add(Gema);
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, GemsInSlots);
	DOREPLIFETIME(UInventoryComponent, GemsItems);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/AttributeComponent.h"
#include "Public/Components/UHealthComponent.h"


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

	

	if (UItemData* Item = Cast<UItemData>(Aitem))
	{
		if (InventoryItems.Find(Item))
		{
			InventoryItems.Add(Item);	
		}else
		{
			GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,"Ya tienes agua");

		}
		
	}
	OnShowItem.Broadcast(InventoryItems);
	OnRep_Items();

}

void UInventoryComponent::OnRep_GemsItems()
{	
	GemsNotDuplicates(GemsItems, GemsItemsInInventory);
	GemsToShow.Broadcast(GemsItemsInInventory);	
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,"Broadcasted Gems item");
}

void UInventoryComponent::OnRep_Items()
{	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
	   TEXT("OnRep_Items ejecutado - Items: ") + FString::FromInt(InventoryItems.Num()));
	OnShowItem.Broadcast(InventoryItems);
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
	if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		Atributos->RecalcularEstadisticas(GemsInSlots);

		if (UUHealthComponent* Health = GetOwner()->FindComponentByClass<UUHealthComponent>())
		{
			if (Atributos->RegenVida > 0.f)
				Health->SetRegenVida(Atributos->RegenVida);
			else
				Health->StopRegenVida();
		}
		
	}
	
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

const TArray<FDatosGema> UInventoryComponent::GetGemsItemsInInventory()
{
	return GemsItemsInInventory;
}

const TArray<UItemData*> UInventoryComponent::GetItemsInInventory()
{
	return InventoryItems;
}

const TArray<FDatosGema> UInventoryComponent::GetGemsItemsInSlots()
{
	return GemsInSlots;
}


void UInventoryComponent::AddGemToSlot(FDatosGema gema)
{
	if (GetOwner()->HasAuthority())
	{
		// Buscar y remover del inventario (solo la primera coincidencia)
		for (int32 i = 0; i < GemsItems.Num(); ++i)
		{
			if (GemsItems[i].NombreGema == gema.NombreGema)
			{
				FName id = GemsItems[i].NombreGema;
				GemsItems.RemoveAt(i);
				GemsNotDuplicates(GemsItems, GemsItemsInInventory);  // esto ya recalcula todo correctamente
				GemsToShow.Broadcast(GemsItemsInInventory);
				break; // ← CRÍTICO: salir tras encontrar la gema
			}
		}

		// Agregar al slot de equipamiento
		GemsToShow.Broadcast(GemsItemsInInventory);

		GemsInSlots.Add(gema);
		GemsToShowInSlots.Broadcast(GemsInSlots);

		// Recalcular estadísticas solo en servidor
		if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
		{
			Atributos->RecalcularEstadisticas(GemsInSlots);

			if (UUHealthComponent* Health = GetOwner()->FindComponentByClass<UUHealthComponent>())
			{
				if (Atributos->RegenVida > 0.f)
					Health->SetRegenVida(Atributos->RegenVida);
				else
					Health->StopRegenVida();
			}
		}
	}
	else
	{
		Server_AddGemToSlot(gema);
	}
}

void UInventoryComponent::Server_QuitGemToSlot_Implementation(const FDatosGema& Gema)
{
	QuitGemToSlot(Gema);
 }

void UInventoryComponent::QuitGemToSlot(FDatosGema gema)
{
	if (GetOwner()->HasAuthority())
	{
		// Buscar y remover del inventario (solo la primera coincidencia)
		for (int32 i = 0; i < GemsInSlots.Num(); ++i)
		{
			if (GemsInSlots[i].NombreGema == gema.NombreGema)
			{
				GemsInSlots.RemoveAt(i);
				break;
			}
		}
		// Recalcular estadísticas solo en servidor
		if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
		{
			Atributos->RecalcularEstadisticas(GemsInSlots);

			if (UUHealthComponent* Health = GetOwner()->FindComponentByClass<UUHealthComponent>())
			{
				if (Atributos->RegenVida > 0.f)
					Health->SetRegenVida(Atributos->RegenVida);
				else
					Health->StopRegenVida();
			}
		}
	}else
	{
		Server_QuitGemToSlot(gema);
	}
}

bool UInventoryComponent::CraftGem(FDatosGema GemaBase, FDatosGema GemaUpgrade)
{
	// Solo el servidor puede modificar el inventario
	if (!GetOwner()->HasAuthority())
	{
		Server_CraftGem(GemaBase, GemaUpgrade);
		// Devolvemos true optimistamente; el servidor validará y replicará el resultado
		return true;
	}

	// Validaciones básicas: la gema debe poder upgradearse y apuntar a la gema correcta
	/*if (GemaBase.ID_Row_NextGema.IsNone() || GemaBase.NeedNextGema <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("CraftGem: La gema '%s' no tiene upgrade definido."), *GemaBase.NombreGema.ToString()));
		return false;
	}*/

	/*if (GemaBase.ID_Row_NextGema != GemaUpgrade.NombreGema)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			TEXT("CraftGem: GemaUpgrade no coincide con ID_NextGema de GemaBase."));
		return false;
	}*/

	// Contamos cuántas copias de GemaBase hay en el inventario
	int32 Cantidad = 0;
	for (const FDatosGema& Gema : GemsItems)
	{
		if (Gema.NombreGema == GemaBase.NombreGema)
		{
			Cantidad++;
		}
	}

	// ¿Hay suficientes materiales?
	if (Cantidad < GemaBase.NeedNextGema)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
			FString::Printf(TEXT("CraftGem: Se necesitan %d de '%s', solo hay %d."),
				GemaBase.NeedNextGema, *GemaBase.NombreGema.ToString(), Cantidad));
		return false;
	}

	// Consumimos exactamente NeedNextGema copias de GemaBase
	int32 AEliminar = GemaBase.NeedNextGema;
	for (int32 i = GemsItems.Num() - 1; i >= 0 && AEliminar > 0; --i)
	{
		if (GemsItems[i].NombreGema == GemaBase.NombreGema)
		{
			GemsItems.RemoveAt(i);
			AEliminar--;
		}
	}

	// Recalculamos el inventario deduplicado y notificamos
	GemsNotDuplicates(GemsItems, GemsItemsInInventory);
	GemsToShow.Broadcast(GemsItemsInInventory);

	// Agregamos la gema resultado del crafteo
	GemsItems.Add(GemaUpgrade);
	GemsNotDuplicates(GemsItems, GemsItemsInInventory);
	GemsToShow.Broadcast(GemsItemsInInventory);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("CraftGem: Gema '%s' crafteada con éxito."), *GemaUpgrade.NombreGema.ToString()));

	return true;
}

void UInventoryComponent::Server_CraftGem_Implementation(FDatosGema GemaBase, FDatosGema GemaUpgrade)
{
	CraftGem(GemaBase, GemaUpgrade);
}

void UInventoryComponent::GemsNotDuplicates(const TArray<FDatosGema>& SourceArray, TArray<FDatosGema>& TargetArray)
{
	TargetArray.Empty();
	TSet<FName> Vistos;

	for (const FDatosGema& Gema : SourceArray)
	{
		if (!Vistos.Contains(Gema.NombreGema))
		{
			Vistos.Add(Gema.NombreGema);
			FDatosGema GemaConCantidad = Gema;
            
			// Contar cuántas veces aparece
			GemaConCantidad.Cantidad = SourceArray.FilterByPredicate([&Gema](const FDatosGema& G)
			{
				return G.NombreGema == Gema.NombreGema;
			}).Num();

			TargetArray.Add(GemaConCantidad);
		}
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, GemsInSlots);
	DOREPLIFETIME(UInventoryComponent, GemsItems);
	DOREPLIFETIME(UInventoryComponent, InventoryItems);

	
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Items/ItemRecolectable.h"

#include "Public/Components/RPGHelper.h"


AItemRecolectable::AItemRecolectable()
{
	mensaje = "Recolectaste";
	cantidad = 1;

}

UItemData* AItemRecolectable::GetDatosDelItem() const
{
	return DatosDelItem;
}

void AItemRecolectable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
// hacer que el otro actor tome los datos del item data
	if (URPGHelper::PickUpItem(OtherActor, DatosDelItem))
	{
		Super::mostrarMensaje(mensaje);
		Destroy();
	}

}

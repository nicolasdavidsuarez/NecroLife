// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Public/ItemRecolectable.h"



AItemRecolectable::AItemRecolectable()
{
	mensaje = "Recolectaste";
	cantidad = 1;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AItemRecolectable::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
		Super::mostrarMensaje(mensaje);
		

		Destroy();


}

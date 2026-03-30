// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Items/ItemDanio.h"


#include "Public/Components/RPGHelper.h"
#include "Public/Components/UHealthComponent.h"


AItemDanio::AItemDanio()
{
	mensaje = "Recibes danio";
	danio = 10;
}

void AItemDanio::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Super::NotifyActorBeginOverlap(OtherActor);
	
	// Reemplaza la l�nea problem�tica por la siguiente para evitar el error de nueva l�nea en la constante
   
	//if (OtherActor->Implements<UPlayerInterface>())
	//{
	//	Super::mostrarMensaje(mensaje);
	//if (OtherActor->)
	//OtherActor->GetComponentByClass<UUHealthComponent>()->TakeDamage(danio);
	URPGHelper::ApplyDamage(OtherActor, danio);
	
		//
	//	IPlayerInterface::Execute_RecibirDanio(OtherActor,danio);
	//}

}

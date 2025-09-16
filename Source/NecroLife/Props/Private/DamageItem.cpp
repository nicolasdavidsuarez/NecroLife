// Fill out your copyright notice in the Description page of Project Settings.



#include "Props/Public/DamageItem.h"

#include "NecroLifeCharacter.h"
#include "NecroLifeCharacter.h"
#include "NecroLifeCharacter.h"
#include "Interfaces/IPluginManager.h"


// Sets default values
ADamageItem::ADamageItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ADamageItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamageItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
//////////////////////////////////
  if (!OtherActor) return;
	
	if (UUHealthComponent* HealthComp = OtherActor->FindComponentByClass<UUHealthComponent>())
	{
		HealthComp->TakeDamage(damage);
		Super::ShowMsg(Msg);
	}
	
}


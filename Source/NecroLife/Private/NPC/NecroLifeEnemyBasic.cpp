// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/NPC/NecroLifeEnemyBasic.h"

#include "Net/UnrealNetwork.h"



// Sets default values
ANecroLifeEnemyBasic::ANecroLifeEnemyBasic()
{
  //////	componente de salud
	HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void ANecroLifeEnemyBasic::OnRep_IsDead()
{
	//
	    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);	
}

void ANecroLifeEnemyBasic::Die() // Función que llamas cuando su vida llega a 0 en el servidor
{
	if (HasAuthority())
	{
		bIsDead = true;
		OnRep_IsDead(); // Forzamos al servidor a que también ejecute la visual
        
		// El servidor destruye el actor después de 3 segundos para dar tiempo a la animación
		SetLifeSpan(3.0f); 
	}
}

void ANecroLifeEnemyBasic::setHealtBar()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("set Healt Bar HealtBar"));
	if(HealthComponent && HealthComponent->MaxHealth > 0)
	{
		float HealthPercent = HealthComponent->CurrentHealth / HealthComponent->MaxHealth;
		BP_SetHealthBar(HealthPercent);
	}
}


void ANecroLifeEnemyBasic::HandleOnHealthChange(float Health, float HealthMax)
{
	float percent=Health/HealthMax;
	BP_SetHealthBar(percent);
}
// Called when the game starts or when spawned
void ANecroLifeEnemyBasic::BeginPlay()
{
	Super::BeginPlay();
if (HealthComponent)
{
	HealthComponent->OnHealthChanged.AddDynamic(this,&ANecroLifeEnemyBasic::HandleOnHealthChange);
}
}

// Called every frame
void ANecroLifeEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANecroLifeEnemyBasic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ANecroLifeEnemyBasic::IsAlive()
{
	if (HealthComponent->IsDead())
	{
		Die();
		return false;
	}
	return true;
}

void ANecroLifeEnemyBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Aquí registramos la variable para que el servidor se la envíe a los clientes
	DOREPLIFETIME(ANecroLifeEnemyBasic, bIsDead);
}


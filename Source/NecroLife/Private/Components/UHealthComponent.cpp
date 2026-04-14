// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/UHealthComponent.h"

#include "Net/UnrealNetwork.h"
#include "NPC/NecroLifeEnemyBasic.h"


// Sets default values for this component's properties
UUHealthComponent::UUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
    SetIsReplicatedByDefault(true);
	
}


// Called when the game starts
void UUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
CurrentHealth = MaxHealth;
	// ...
	
}


// Called every frame
void UUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUHealthComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	/*ANecroLifeEnemyBasic* Owner = Cast<ANecroLifeEnemyBasic>(GetOwner());
	if (Owner)
	{
		Owner->setHealtBar();
	}*/
}

void UUHealthComponent::TakeDamage(float Amount)
{
	if (GetOwnerRole() < ROLE_Authority) return;
	
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0.f;
		//disparar evento para cuando muere
		
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyHealing(float Amount)
{
	if (Amount+CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}else
	{
		CurrentHealth += Amount;
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyDamageOverTime(float DamagePerTick, float Interval, float TotalDuration)
{
	if(DamagePerTick <= 0.f||Interval <= 0.f||TotalDuration <= 0.f) return;
	DamageOverTime=DamagePerTick;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DOT,this,&UUHealthComponent::ApplyDamageOverTimer,Interval,true);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyDamageOverTimer()
{
	TakeDamage(DamageOverTime);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUHealthComponent, CurrentHealth);
	DOREPLIFETIME(UUHealthComponent, MaxHealth);
	
}
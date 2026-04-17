// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Components/UHealthComponent.h"
#include "Public/Components/AttributeComponent.h"



// Sets default values for this component's properties
UUHealthComponent::UUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	// Iniciamos la regen base leyendo el AttributeComponent
	// Si BaseRegenVida = 0 no hace nada, si tiene valor arranca el timer
	if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		if (Atributos->BaseRegenVida > 0.f)
		{
			SetRegenVida(Atributos->BaseRegenVida);
		}
	}
}


// Called every frame
void UUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUHealthComponent::TakeDamage(float Amount)
{
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

void UUHealthComponent::SetRegenVida(float PorcentajePorSegundo, float Intervalo)
{
	// Si ya había un timer corriendo, lo cancelamos antes de crear uno nuevo
	StopRegenVida();

	if (PorcentajePorSegundo <= 0.f) return;

	RegenPorcentaje = PorcentajePorSegundo;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Regen, this, &UUHealthComponent::AplicarRegenVida, Intervalo, true);
}

void UUHealthComponent::StopRegenVida()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Regen);
	RegenPorcentaje = 0.f;
}

void UUHealthComponent::AplicarRegenVida()
{
	// Solo regenera si no está llena la vida
	if (CurrentHealth < MaxHealth)
	{
		ApplyHealing(MaxHealth * RegenPorcentaje);
	}
}


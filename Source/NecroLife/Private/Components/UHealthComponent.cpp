// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Components/UHealthComponent.h"
#include "Public/Components/AttributeComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UUHealthComponent::UUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;

	// Iniciamos la regen base leyendo el AttributeComponent.
	// Si BaseRegenVida = 0 no hace nada, si tiene valor arranca el timer.
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
}

void UUHealthComponent::OnRep_CurrentHealth()
{
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	// Descomentá esto si necesitás actualizar la barra de vida del enemigo en el cliente:
	// ANecroLifeEnemyBasic* Owner = Cast<ANecroLifeEnemyBasic>(GetOwner());
	// if (Owner) Owner->setHealtBar();
}

void UUHealthComponent::TakeDamage(float Amount)
{
	// Solo el servidor aplica daño (authority check).
	if (GetOwnerRole() < ROLE_Authority) return;

	// Reducimos el daño según la defensa del AttributeComponent.
	// Fórmula: DañoFinal = Daño * (100 / (100 + Defensa))
	// Con Defensa=40: 40 * (100/140) = ~28. Con Defensa=0: daño completo.
	if (UAttributeComponent* Atributos = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		Amount = Amount * (100.0f / (100.0f + Atributos->Defense));
	}

	CurrentHealth -= Amount;
	if (CurrentHealth <= 0.f)
	{
		OnDie.Broadcast();
		CurrentHealth = 0.f;
		// TODO: disparar evento para cuando muere
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyHealing(float Amount)
{
	if (Amount + CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
		CurrentHealth += Amount;
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyDamageOverTime(float DamagePerTick, float Interval, float TotalDuration)
{
	if (DamagePerTick <= 0.f || Interval <= 0.f || TotalDuration <= 0.f) return;

	DamageOverTime = DamagePerTick;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_DOT, this, &UUHealthComponent::ApplyDamageOverTimer, Interval, true);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::ApplyDamageOverTimer()
{
	TakeDamage(DamageOverTime);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UUHealthComponent::SetRegenVida(float PorcentajePorSegundo, float Intervalo)
{
	StopRegenVida();
	if (PorcentajePorSegundo <= 0.f) return;

	RegenPorcentaje = PorcentajePorSegundo;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Regen, this, &UUHealthComponent::AplicarRegenVida, Intervalo, true);
}

void UUHealthComponent::StopRegenVida()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Regen);
	RegenPorcentaje = 0.f;
}

void UUHealthComponent::AplicarRegenVida()
{
	// Solo regenera si no está llena la vida.
	if (CurrentHealth < MaxHealth)
	{
		ApplyHealing(MaxHealth * RegenPorcentaje);
	}
}

void UUHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUHealthComponent, CurrentHealth);
	DOREPLIFETIME(UUHealthComponent, MaxHealth); // Agregado por networking
}
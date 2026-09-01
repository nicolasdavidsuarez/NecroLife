// Fill out your copyright notice in the Description page of Project Settings.
// En esta clase se tratara de hacer un componente modular que se atache a el character, los npc, y objetos que
// sean destructibles... (puertas, puentes, torres)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "UHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//////////Todas las variables se tratan de escribir en ingles/////////////////////////////////

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDie OnDie;

	// Replicado para que todos los clientes vean el MaxHealth correcto.
	// BlueprintReadWrite para poder configurarlo desde Blueprint/editor.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.f;

	// ReplicatedUsing dispara OnRep en clientes cuando cambia el valor.
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth, VisibleAnywhere, BlueprintReadOnly, Category="Health")
	float CurrentHealth;

	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyHealing(float Amount);

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead() const { return CurrentHealth <= 0.f; }

	// --- Daño en el tiempo: veneno, fuego, sol, etc.
	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamageOverTime(float DamagePerTick, float Interval, float TotalDuration);

	// Activa la regeneración de vida con un intervalo en segundos
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetRegenVida(float PorcentajePorSegundo, float Intervalo = 1.0f);

	// Detiene la regeneración de vida
	UFUNCTION(BlueprintCallable, Category="Health")
	void StopRegenVida();

	//////////Variables privadas
	// TimerHandle: administrador de tiempo de Unreal.
	// Sufijo _DOT = Damage Over Time (término estándar en videojuegos).
private:
	FTimerHandle TimerHandle_DOT;
	float AmountToDamage_DOT;
	float AmountTime;
	float DamageOverTime;

	FTimerHandle TimerHandle_Regen;
	float RegenPorcentaje = 0.f;

	void ApplyDamageOverTimer();
	void AplicarRegenVida();
};
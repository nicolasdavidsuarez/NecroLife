// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Public/Components/UHealthComponent.h"
#include "Components/WidgetComponent.h" // Agregado por compañeros
#include "NecroLifeEnemyBasic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, ANecroLifeEnemyBasic*, Enemy);

class UUHealthComponent;

UCLASS()
class NECROLIFE_API ANecroLifeEnemyBasic : public ACharacter
{
	GENERATED_BODY()

public:
	ANecroLifeEnemyBasic();

	// --- Lógica de Salud y Networking (Tu avance) ---
	UPROPERTY(ReplicatedUsing = OnRep_IsDead)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_IsDead();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void HandleOnHealthChange(float Health, float HealthMax);

	UFUNCTION(BlueprintImplementableEvent, Category = "Event")
	void BP_SetHealthBar(float Percent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Event")
	void BP_OnDie();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnEnemyDied OnEnemyDied;

// --- Variables de Animación ---
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Animacion")
	bool bIsEAttacking = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Animacion")
	bool bIsEDamaged = false;
	
	// Función para que el AnimBP avise que la animación terminó
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Animacion")
	void Server_ResetDamageState();
	
	// --- Componentes y Tags (Ambos) ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UUHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	FGameplayTag Tag;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTag() { return Tag; }

	UFUNCTION(BlueprintCallable)
	void SetTag(FGameplayTag newTag) { Tag = newTag; }

	// Esencias que otorga al morir (configurable por instancia en Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Esencias")
	float EsenciasAlMorir = 60.f;

	// --- Sistema de Targeting (Avance de compañeros) ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting")
	class UWidgetComponent* TargetWidget;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Targeting")
	void OnTargetStatusChanged(bool bIsTarget);
	
	// --- Configuración de Ataque Melee ---
	
	// El nombre del socket o hueso desde donde sale el ataque (ej. "MouthSocket", "Hand_R")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
	FName AttackSocketName = FName("AttackSocket");

	// Qué tan grande es la zona de impacto del golpe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
	float AttackRadius = 45.f;

	// Daño unificado para todos los enemigos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
	float AttackDamage = 20.f;

	// Función que dispara el AnimBP en el frame del golpe
	UFUNCTION(BlueprintCallable, Category = "Combate")
	void ExecuteMeleeAttack();
	
	// --- Sistema de Estados (Crowd Control) ---
    
	// Tildar esto en el Blueprint del Jefe para que sea inmune
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Config")
	bool bIsBoss = false;

	// Estado actual del enemigo
	UPROPERTY(BlueprintReadOnly, Category = "Combat|State")
	bool bIsStunned = false;
	
	// Memoria para restaurar la velocidad después del aturdimiento
	UPROPERTY()
	float DefaultWalkSpeed = 0.0f;

	// Cronómetro para saber cuándo devolverle la movilidad
	FTimerHandle StunTimerHandle;

	// Función que Huesos llamará para atraparlo
	UFUNCTION(BlueprintCallable, Category = "Combat|Abilities")
	void Immobilize(float Duration);

	// Función interna para liberarlo
	void ClearImmobilize();
	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool IsAlive();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Widget components que deben mirar siempre a la cámara (health bars del BP)
	TArray<UWidgetComponent*> BillboardWidgets;
};
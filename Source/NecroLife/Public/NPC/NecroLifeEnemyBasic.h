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

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool IsAlive();
};
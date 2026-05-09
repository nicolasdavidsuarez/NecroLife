// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Public/Components/UHealthComponent.h"
#include "Components/WidgetComponent.h" // Agregado por compañeros
#include "NecroLifeEnemyBasic.generated.h"

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

	// --- Componentes y Tags (Ambos) ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UUHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	FGameplayTag Tag;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTag() { return Tag; }

	UFUNCTION(BlueprintCallable)
	void SetTag(FGameplayTag newTag) { Tag = newTag; }

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
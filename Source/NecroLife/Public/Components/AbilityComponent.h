// Fill out your copyright notice in the Description page of Project Settings.
//AbilityComponent componente de habilidades, basado en GAS
//
// GAS organiza la lógica de las acciones en 3 pasos:
//   Input -> Ability -> Outcome
// - Input: la entrada del jugador (ej: tecla Q, click).
// - Ability: la lógica que valida costos, cooldowns y condiciones.
// - Outcome: el resultado en el juego (daño, curación, animación).
//
// Este componente maneja la lista de habilidades del personaje,
// verifica si pueden ejecutarse y las activa, aplicando sus efectos.

#pragma once

#include "CoreMinimal.h"
#include "Data/AbilityData.h"
#include "NiagaraComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownAbility, int, AbilitySlot);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();
     //////////////////////////////////////////////////////////////////
	// Lista de habilidades disponibles
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<UAbilityData*> Abilities;

	// Habilidad actualmente seleccionada
	UPROPERTY(BlueprintReadOnly)
	UAbilityData* CurrentAbility;

	UPROPERTY(BlueprintAssignable, Category="Abilities")
	FCoolDownAbility AbilitySlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;

	// Componente visual de Niagara activo
	UPROPERTY()
	UNiagaraComponent* ActiveIndicator;

	UFUNCTION(BLueprintCallable, Category="Abilities")
	void AbilityAply();

	UFUNCTION(BLueprintCallable, Category="Abilities")
	bool isCoolDownAply(UAbilityData* Ability);

	UFUNCTION(BLueprintCallable, Category="Abilities")
	TArray<UAbilityData*> getAbilitySlots();

protected:
	// Mapa para almacenar en qué segundo del juego termina el cooldown de cada habilidad
	UPROPERTY(BlueprintReadOnly, Category="Abilities")
	TMap<UAbilityData*, float> AbilityCooldownEndTimes;

public:	
	int32 CurrentAbilitySlot;
	void SelectAbility(int32 Index);
	void InitPreview();
	void UpdatePreview(FVector posicionPointer);
	void UpdateIndicator(const FVector& TargetLocation);
	void ClearIndicator();
	
////////////////////////////////////////////////////////////////////
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

   
};

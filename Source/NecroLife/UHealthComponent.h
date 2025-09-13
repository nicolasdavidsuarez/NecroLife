// Fill out your copyright notice in the Description page of Project Settings.
//En esta clase se tratara de hacer un componente modular que se atache a el character, los npc, y objetos que
// sean destructibles... (puertas, puentes, torres)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "UHealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//////////Todas las variables se tratan de escribir en ingles/////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	float CurrentHealth;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float Amount);

	//lleva const delante para garantizar que el metodo no cambia los valores. 
	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead() const { return CurrentHealth <= 0.f; }

	// --- Daño en el tiempo para veneno, fuego, sol?
	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamageOverTime(float DamagePerTick, float Interval, float TotalDuration);

	//////////Variables privadas

	///TImer handle es un administrador de tiempo de unreal
    ///Sufijo _DOT lo pongo de DAMAGE OVER TIME se usa en video juegos
	private:
	FTimerHandle TimerHandle_DOT;
	float AmountToDamage_DOT;
	float AmountTime;
     float DamageOverTime;
	
	void ApplyDamageOverTimer();
};

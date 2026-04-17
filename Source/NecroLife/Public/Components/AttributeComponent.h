// Fill out your copyright notice in the Description page of Project Settings.
//clase atributos, basado en GAS game abitlity system
//
// GAS organiza la lógica de las acciones en 3 pasos:
//   Input -> Ability -> Outcome
// - Input: la entrada del jugador (ej: tecla Q, click).
// - Ability: la lógica que valida costos, cooldowns y condiciones.
// - Outcome: el resultado en el juego (daño, curación, animación).
//
// Este componente maneja los atributos del personaje,
// Las habilidades consultan este componente para verificar costos y aplicar efectos.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


struct FDatosGema;

USTRUCT(BlueprintType)
struct FEstadisticasPersonaje
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	int32 VidaMaxima;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	int32 Ataque;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	int32 Defensa;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	float Velocidad;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	int32 EnergiaMaxima;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	float VelocidadRegeneracion;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	int32 Nivel;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	float RegenVida;

	UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
	float RegenEnergia;

	// Agregá acá todos los atributos que quieras mostrar en pantalla
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPChanged, float, XP, float, XPtoNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtributosActualizados, const FEstadisticasPersonaje&, NuevosAtributos);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributeComponent();

	UPROPERTY(BlueprintAssignable, Category="Atributos|Eventos")
	FOnAtributosActualizados OnAtributosActualizados;

	//////Level

	UFUNCTION(BlueprintCallable, Category="Level")
	void TakeXP(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	float XP=0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	float XPtoNextLevel=100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 Level=0;

	//////Life

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
	int32 Life=100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Life")
	int32 BaseLife=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
	int32 LifeMax=100;

	//////Energy

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 Energy=100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
	int32 BaseEnergy=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 EnergyMax=100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
	int32 EnergyReg=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
	float velocidadEnergyReg=1;

	//////Velocity

	// Valor final calculado (base + gemas). Solo lectura.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	float Velocity=10;

	// Velocidad base sin gemas. Modificable desde el Editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Velocity")
	float BaseVelocity=10;

	//////Attack

	// Valor final calculado (base + gemas). Solo lectura.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	int32 Attack=10;

	// Ataque base sin gemas. Modificable desde el Editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	int32 BaseAttack=40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	float VelocityAttackBase=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	float VelocityAttack=1;

	//////Defense

	// Defensa base sin gemas. Modificable desde el Editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defense")
	int32 DefenseBase=40;

	// Valor final calculado (base + gemas). Solo lectura.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defense")
	int32 Defense=0;

	//////Regen

	// Regen de vida base sin gemas. Modificable desde el Editor (ej: 1 = 1% por segundo).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regen")
	float BaseRegenVida = 0.0f;

	// Valor final calculado (base + gemas). Solo lectura.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Regen")
	float RegenVida = 0.0f;

	// Regen de energía base sin gemas. Modificable desde el Editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regen")
	float BaseRegenEnergia = 0.0f;

	// Valor final calculado (base + gemas). Solo lectura.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Regen")
	float RegenEnergia = 0.0f;

	//////Dash

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashStrength = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashCooldown = 1.0f;

	UPROPERTY(BlueprintAssignable, Category="XP_Level")
	FOnXPChanged OnXPChanged;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Atributos|Calculos")
	void RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas);

};

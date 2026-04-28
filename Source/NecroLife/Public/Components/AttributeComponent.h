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

	// Agregá acá todos los atributos que quieras mostrar en pantalla
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXPChanged, float, XP, float, XPtoNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtributosActualizados, const FEstadisticasPersonaje&, NuevosAtributos);
// Arriba del UCLASS
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPChanged, float, CurrentXP, float, XPToNextLevel, int32, CurrentLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(ReplicatedUsing = OnRep_StatsActualizadas)
	FEstadisticasPersonaje StatsSincronizadas;

	
	UFUNCTION()
	void OnRep_StatsActualizadas();

	
	
	// Sets default values for this component's properties
	UAttributeComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashStrength = 1500.f;

	UPROPERTY(BlueprintAssignable, Category="Atributos|Eventos")
	FOnAtributosActualizados OnAtributosActualizados;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
	float DashCooldown = 1.0f;

	//////Level

	UFUNCTION(BlueprintCallable, Category="Level")
	void TakeXP(float Amount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	float XP=0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	float XPtoNextLevel=100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 Level=0;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
	int32 Life=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
	int32 BaseLife=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
	int32 LifeMax=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 Energy=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 BaseEnergy=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 EnergyMax=100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	int32 EnergyReg=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
	float velocidadEnergyReg=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	float Velocity=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
	float BaseVelocity=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	int32 Attack=10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	int32 BaseAttack=10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	float VelocityAttackBase=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
	float VelocityAttack=1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defense")
	int32 DefenseBase=0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defense")
	int32 Defense=0;

	
	

	UPROPERTY(BlueprintAssignable, Category = "NecroLife | Attributes")
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

	UFUNCTION(Server,Reliable)
	void Server_RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas);
	
	
};

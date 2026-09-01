// Fill out your copyright notice in the Description page of Project Settings.
// clase atributos, basado en GAS game ability system

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

    // De los compañeros: stats de regen para la UI
    UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
    float RegenVida;

    UPROPERTY(BlueprintReadOnly, Category="Estadisticas")
    float RegenEnergia;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtributosActualizados, const FEstadisticasPersonaje&, NuevosAtributos);
// Level agregado por networking
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPChanged, float, CurrentXP, float, XPToNextLevel, int32, CurrentLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAttributeComponent();

    // --- Replicación (networking) ---
    UPROPERTY(ReplicatedUsing=OnRep_StatsActualizadas)
    FEstadisticasPersonaje StatsSincronizadas;
    
    UFUNCTION(BlueprintCallable)
    FEstadisticasPersonaje GetStatsSincronizadas();

    UFUNCTION()
    void OnRep_StatsActualizadas();

    UFUNCTION()
    void OnRep_XPChanged();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // --- Eventos ---
    UPROPERTY(BlueprintAssignable, Category="Atributos|Eventos")
    FOnAtributosActualizados OnAtributosActualizados;

    UPROPERTY(BlueprintAssignable, Category="NecroLife|Attributes")
    FOnXPChanged OnXPChanged;

    // --- Level / XP ---
    static constexpr int32 MaxLevel = 20;
    static const float XPTable[MaxLevel];

    UFUNCTION(BlueprintCallable, Category="Level")
    void TakeXP(float Amount);

    UPROPERTY(ReplicatedUsing=OnRep_XPChanged, VisibleAnywhere, BlueprintReadOnly, Category="Level")
    float XP = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
    float XPtoNextLevel = 100.f;

    UPROPERTY(ReplicatedUsing=OnRep_XPChanged, VisibleAnywhere, BlueprintReadOnly, Category="Level")
    int32 Level = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level|Progresion")
    int32 VidaPorNivel = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level|Progresion")
    int32 AtaquePorNivel = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level|Progresion")
    int32 DefensaPorNivel = 3;

    // --- Life ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
    int32 Life = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Life")
    int32 BaseLife = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Life")
    int32 LifeMax = 100;

    // --- Energy ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
    int32 Energy = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
    int32 BaseEnergy = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Energy")
    int32 EnergyMax = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
    int32 EnergyReg = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Energy")
    float velocidadEnergyReg = 1;

    // --- Velocity ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Velocity")
    float Velocity = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Velocity")
    float BaseVelocity = 10;

    // --- Attack ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
    int32 Attack = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    int32 BaseAttack = 40;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    float VelocityAttackBase = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack")
    float VelocityAttack = 1;

    // --- Defense ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defense")
    int32 DefenseBase = 40;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defense")
    int32 Defense = 0;

    // --- Regen (de los compañeros, necesario para HealthComponent) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regen")
    float BaseRegenVida = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Regen")
    float RegenVida = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Regen")
    float BaseRegenEnergia = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Regen")
    float RegenEnergia = 0.0f;

    // --- Dash ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
    float DashStrength = 1500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
    float DashDuration = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dash")
    float DashCooldown = 1.0f;

    // --- Funciones ---
    UFUNCTION(BlueprintCallable, Category="Atributos|Calculos")
    void RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas);

    UFUNCTION(Server, Reliable)
    void Server_RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas);

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
};
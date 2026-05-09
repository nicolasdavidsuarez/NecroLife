// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Components/AttributeComponent.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
    
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Regeneración de energía por frame (de los compañeros).
    // Solo corre en el servidor para evitar desync.
    if (GetOwner()->HasAuthority() && RegenEnergia > 0.f && Energy < EnergyMax)
    {
        Energy = FMath::Min(EnergyMax, FMath::RoundToInt(Energy + EnergyMax * RegenEnergia * DeltaTime));
    }
}

void UAttributeComponent::TakeXP(float Amount)
{
    // Authority check (networking)
    if (!GetOwner()->HasAuthority()) return;

    if (XP + Amount > XPtoNextLevel)
    {
        Level++;
        XP = XPtoNextLevel + Amount - XPtoNextLevel;
    }
    else
    {
        XP += Amount;
    }
    OnXPChanged.Broadcast(XP, XPtoNextLevel, Level);
}

void UAttributeComponent::RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas)
{
    if (GetOwner()->HasAuthority())
    {
        // 1. Resetear a valores base
        LifeMax = BaseLife;
        Attack = BaseAttack;
        VelocityAttack = VelocityAttackBase;
        Defense = DefenseBase;
        Velocity = BaseVelocity;
        EnergyMax = BaseEnergy;
        RegenVida = BaseRegenVida;
        RegenEnergia = BaseRegenEnergia;

        // Acumuladores porcentuales
        float BonoVidaPorcentaje = 0.0f;
        float BonoAtaquePorcentaje = 0.0f;
        float BonoVelocidadAttack = 0.0f;
        float BonoDefensaPorcentaje = 0.0f;
        float BonoVelocidadPorcentaje = 0.0f;
        float BonoRegenVidaPorcentaje = 0.0f;
        float BonoRegenEnergiaPorcentaje = 0.0f;

        // 2. Iterar gemas
        for (const FDatosGema& Gema : GemasEquipadas)
        {
            switch (Gema.AtributoAMejorar)
            {
            case ETipoEstadisticaGema::VidaMaxima:
                LifeMax += FMath::RoundToInt(Gema.ValorMejora);
                break;

            case ETipoEstadisticaGema::AtaqueFisico:
                Attack += FMath::RoundToInt(Gema.ValorMejora);
                break;

            case ETipoEstadisticaGema::Defensa:
                Defense += Gema.ValorMejora;
                break;

            case ETipoEstadisticaGema::EnergiaMaxima:
                EnergyMax += FMath::RoundToInt(Gema.ValorMejora);
                break;

            case ETipoEstadisticaGema::VidaMaximaPorcentaje:
                BonoVidaPorcentaje += (Gema.ValorMejora - 1.0f);
                break;

            case ETipoEstadisticaGema::AtaqueFisicoPorcentaje:
                BonoAtaquePorcentaje += (Gema.ValorMejora - 1.0f);
                break;

            case ETipoEstadisticaGema::DefensaPorcentaje:
                BonoDefensaPorcentaje += (Gema.ValorMejora - 1.0f);
                break;

            case ETipoEstadisticaGema::Velocidad:
                BonoVelocidadPorcentaje += Gema.ValorMejora / 100.0f;
                break;

            case ETipoEstadisticaGema::VelocidadAtaquePorcentaje:
                BonoVelocidadAttack += (Gema.ValorMejora - 1.0f);
                break;

            case ETipoEstadisticaGema::RegeneracionVida:
                BonoRegenVidaPorcentaje += Gema.ValorMejora / 100.0f;
                break;

            case ETipoEstadisticaGema::RegeneracionEnergia:
                BonoRegenEnergiaPorcentaje += Gema.ValorMejora / 100.0f;
                break;

            default:
                break;
            }
        }

        // 3. Aplicar bonos porcentuales
        if (BonoVidaPorcentaje > 0.0f)
            LifeMax *= (1.0f + BonoVidaPorcentaje);

        if (BonoAtaquePorcentaje > 0.0f)
            Attack = Attack * (1 + BonoAtaquePorcentaje);

        if (BonoVelocidadAttack > 0.0f)
            VelocityAttack = FMath::RoundToInt(VelocityAttack * BonoVelocidadAttack);

        if (BonoDefensaPorcentaje > 0.0f)
            Defense += FMath::RoundToInt(Defense * BonoDefensaPorcentaje);

        Velocity += Velocity * BonoVelocidadPorcentaje;
        RegenVida += RegenVida * BonoRegenVidaPorcentaje;
        RegenEnergia += RegenEnergia * BonoRegenEnergiaPorcentaje;

        if (Life > LifeMax)
            Life = LifeMax;

        // Armar struct para replicación y UI
        StatsSincronizadas.VidaMaxima = LifeMax;
        StatsSincronizadas.Ataque = Attack;
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
            FString::Printf(TEXT("Attack: %d"),Attack));
        StatsSincronizadas.Defensa = Defense;
        StatsSincronizadas.Velocidad = Velocity;
        StatsSincronizadas.Nivel = Level;
        StatsSincronizadas.EnergiaMaxima = EnergyMax;
        StatsSincronizadas.VelocidadRegeneracion = velocidadEnergyReg;
        StatsSincronizadas.RegenVida = RegenVida;
        StatsSincronizadas.RegenEnergia = RegenEnergia;
    }
    else
    {
        // Cliente: delega al servidor
        Server_RecalcularEstadisticas(GemasEquipadas);
    }

    OnRep_StatsActualizadas();
}

void UAttributeComponent::Server_RecalcularEstadisticas_Implementation(const TArray<FDatosGema>& GemasEquipadas)
{
    RecalcularEstadisticas(GemasEquipadas);
    OnRep_StatsActualizadas();
}

void UAttributeComponent::OnRep_StatsActualizadas()
{
    OnAtributosActualizados.Broadcast(StatsSincronizadas);
}

void UAttributeComponent::OnRep_XPChanged()
{
    OnXPChanged.Broadcast(XP, XPtoNextLevel, Level);
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UAttributeComponent, StatsSincronizadas);
    DOREPLIFETIME(UAttributeComponent, XP);
    DOREPLIFETIME(UAttributeComponent, Level);
}
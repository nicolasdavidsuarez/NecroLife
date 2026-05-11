// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Components/AttributeComponent.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

const float UAttributeComponent::XPTable[UAttributeComponent::MaxLevel] = {
    100.f,    // 0 → 1  (~2 kills)
    160.f,    // 1 → 2  (~3 kills)
    220.f,    // 2 → 3  (~4 kills)
    280.f,    // 3 → 4  (~5 kills)
    500.f,    // 4 → 5
    750.f,    // 5 → 6
    1000.f,   // 6 → 7
    1400.f,   // 7 → 8
    1900.f,   // 8 → 9
    2500.f,   // 9 → 10
    3200.f,   // 10 → 11
    4000.f,   // 11 → 12
    4900.f,   // 12 → 13
    5900.f,   // 13 → 14
    7000.f,   // 14 → 15
    8200.f,   // 15 → 16
    9500.f,   // 16 → 17
    11000.f,  // 17 → 18
    12500.f,  // 18 → 19
    14000.f,  // 19 → 20
};

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner()->HasAuthority())
    {
        StatsSincronizadas.VidaMaxima          = BaseLife;
        StatsSincronizadas.Ataque              = BaseAttack;
        StatsSincronizadas.Defensa             = DefenseBase;
        StatsSincronizadas.Velocidad           = BaseVelocity;
        StatsSincronizadas.EnergiaMaxima       = BaseEnergy;
        StatsSincronizadas.VelocidadRegeneracion = velocidadEnergyReg;
        StatsSincronizadas.Nivel               = Level;
        StatsSincronizadas.RegenVida           = BaseRegenVida;
        StatsSincronizadas.RegenEnergia        = BaseRegenEnergia;

        OnRep_StatsActualizadas();
    }
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
    if (!GetOwner()->HasAuthority()) return;
    if (Level >= MaxLevel) return;

    XP += Amount;

    while (XP >= XPtoNextLevel && Level < MaxLevel)
    {
        XP -= XPtoNextLevel;
        Level++;

        BaseLife    += VidaPorNivel;
        BaseAttack  += AtaquePorNivel;
        DefenseBase += DefensaPorNivel;

        if (Level < MaxLevel)
            XPtoNextLevel = XPTable[Level];
        else
            XPtoNextLevel = 0.f;
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
                RegenVida += Gema.ValorMejora;
                break;

            case ETipoEstadisticaGema::RegeneracionEnergia:
                RegenEnergia += Gema.ValorMejora;
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
            VelocityAttack = VelocityAttack * (1.0f + BonoVelocidadAttack);

        if (BonoDefensaPorcentaje > 0.0f)
            Defense += FMath::RoundToInt(Defense * BonoDefensaPorcentaje);

        Velocity += Velocity * BonoVelocidadPorcentaje;

        if (Life > LifeMax)
            Life = LifeMax;

        // Armar struct para replicación y UI
        StatsSincronizadas.VidaMaxima = LifeMax;
        StatsSincronizadas.Ataque = Attack;
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
            FString::Printf(TEXT("Attack: %d  |  Velocity: %.2f  |  MaxWalkSpeed aprox: %.0f"), Attack, Velocity, Velocity * 50.f));
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
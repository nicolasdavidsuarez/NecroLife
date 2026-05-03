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


#include "Public/Components/AttributeComponent.h"

#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}
	// ...



void UAttributeComponent::TakeXP(float Amount)
{
	if (!GetOwner()->HasAuthority()) return;
	if (XP+Amount>XPtoNextLevel)
	{
		Level++;
		XP=XPtoNextLevel+Amount-XPtoNextLevel;
	}else
	{
		XP += Amount;
	}
	OnXPChanged.Broadcast(XP,XPtoNextLevel,Level);
}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas)
{
	if (GetOwner()->HasAuthority())
	{
		// 1. Resetear las estadísticas a los valores "desnudos"
	LifeMax = BaseLife;
	Attack = BaseAttack;
	VelocityAttack=VelocityAttackBase;
	Defense=DefenseBase;
	// ... resetear el resto ...

	// Variables temporales para acumular los porcentajes y aplicarlos al final
	float BonoVidaPorcentaje = 0.0f;
	float BonoAtaquePorcentaje = 0.0f;
	float BonoVelocidadAttack=0.0f;
	float BonoDefense=0.0f;

	// 2. Iterar por todas las gemas que están físicamente en los slots
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

		case ETipoEstadisticaGema::VidaMaximaPorcentaje:
			// Sumamos el porcentaje (ej: si tiene dos gemas de 10%, esto acumula 20%)
			BonoVidaPorcentaje += (Gema.ValorMejora-1.0f);
			break;

		case ETipoEstadisticaGema::AtaqueFisicoPorcentaje:
			BonoAtaquePorcentaje += (Gema.ValorMejora-1.0f);
			break;

		case ETipoEstadisticaGema::VelocidadAtaquePorcentaje:
			BonoVelocidadAttack+=(Gema.ValorMejora-1.0f);
			break;

		case ETipoEstadisticaGema::Defensa:
			Defense+=Gema.ValorMejora;
			break;
        case ETipoEstadisticaGema::DefensaPorcentaje:
			BonoDefense+=(Gema.ValorMejora-1.0f);
			break;
		
		default:
			break;

			// ... agregar los demás casos del Enum ...
		}
	}

	// 3. Aplicar los bonos porcentuales al final (para que matemáticamente sea correcto)
	//aca hay que ponerse de acuerdo, o ponemos 1.1x o 10%
	//lo dejo mal para tener las dos opciones
		if (BonoVidaPorcentaje > 0.0f)
		{
			LifeMax *= (1.0f + BonoVidaPorcentaje);
		}
	if (BonoVelocidadAttack>0.0f)
	{
		VelocityAttack=FMath::RoundToInt(VelocityAttack*BonoVelocidadAttack);
	}
	if(BonoAtaquePorcentaje>0.0f)
	{
	Attack=Attack*(1+BonoAtaquePorcentaje);	
	}

	// Opcional: Asegurarse de que la Vida actual no supere la nueva Vida Máxima
	if (Life > LifeMax)
	{
		Life = LifeMax;
	}

	// Al final de la función, armamos el struct para la UI

	StatsSincronizadas.VidaMaxima = LifeMax;
	StatsSincronizadas.Ataque = Attack;
	StatsSincronizadas.Velocidad = Velocity;
	StatsSincronizadas.Nivel = Level;
	StatsSincronizadas.EnergiaMaxima=EnergyMax;
	StatsSincronizadas.VelocidadRegeneracion=velocidadEnergyReg;
	StatsSincronizadas.Defensa=Defense;

	
	}else
	{
		Server_RecalcularEstadisticas(GemasEquipadas);		
		
	}
	//OnAtributosActualizados.Broadcast(StatsSincronizadas);
	OnRep_StatsActualizadas();
}

void UAttributeComponent::OnRep_XPChanged()
{
	OnXPChanged.Broadcast(XP, XPtoNextLevel, Level);
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

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAttributeComponent, StatsSincronizadas);
	DOREPLIFETIME(UAttributeComponent, XP);
	DOREPLIFETIME(UAttributeComponent, Level);
	//DOREPLIFETIME(UAttributeComponent, XPtoNextLevel);
}

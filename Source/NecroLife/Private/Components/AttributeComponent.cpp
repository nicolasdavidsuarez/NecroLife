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


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}
	// ...



void UAttributeComponent::TakeXP(float Amount)
{

	if (XP+Amount>XPtoNextLevel)
	{
		Level++;
		XP=XPtoNextLevel+Amount-XPtoNextLevel;
	}else
	{
		XP += Amount;
	}
	OnXPChanged.Broadcast(XP,XPtoNextLevel);
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

	// Regeneración de energía: se aplica por frame usando el % calculado por gemas
	// Con RegenEnergia = 0 (sin gema) no hace nada
	if (RegenEnergia > 0.f && Energy < EnergyMax)
	{
		Energy = FMath::Min(EnergyMax, FMath::RoundToInt(Energy + EnergyMax * RegenEnergia * DeltaTime));
	}
}

void UAttributeComponent::RecalcularEstadisticas(const TArray<FDatosGema>& GemasEquipadas)
{
	// 1. Resetear las estadísticas a los valores "desnudos"
	LifeMax = BaseLife;
	Attack = BaseAttack;
	VelocityAttack = VelocityAttackBase;
	Defense = DefenseBase;
	Velocity = BaseVelocity;
	EnergyMax = BaseEnergy;
	RegenVida = BaseRegenVida;
	RegenEnergia = BaseRegenEnergia;

	// Variables temporales para acumular los porcentajes y aplicarlos al final
	float BonoVidaPorcentaje = 0.0f;
	float BonoAtaquePorcentaje = 0.0f;
	float BonoDefensaPorcentaje = 0.0f;
	float BonoVelocidadPorcentaje = 0.0f;
	float BonoRegenVidaPorcentaje = 0.0f;
	float BonoRegenEnergiaPorcentaje = 0.0f;

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

		case ETipoEstadisticaGema::Defensa:
			Defense += FMath::RoundToInt(Gema.ValorMejora);
			break;

		case ETipoEstadisticaGema::EnergiaMaxima:
			EnergyMax += FMath::RoundToInt(Gema.ValorMejora);
			break;

		// --- Porcentuales: ValorMejora se configura como número entero (ej: 10 = 10%) ---
		case ETipoEstadisticaGema::VidaMaximaPorcentaje:
			BonoVidaPorcentaje += Gema.ValorMejora / 100.0f;
			break;

		case ETipoEstadisticaGema::AtaqueFisicoPorcentaje:
			BonoAtaquePorcentaje += Gema.ValorMejora / 100.0f;
			break;

		case ETipoEstadisticaGema::DefensaPorcentaje:
			BonoDefensaPorcentaje += Gema.ValorMejora / 100.0f;
			break;

		case ETipoEstadisticaGema::Velocidad:
			BonoVelocidadPorcentaje += Gema.ValorMejora / 100.0f;
			break;

		case ETipoEstadisticaGema::RegeneracionVida:
			BonoRegenVidaPorcentaje += Gema.ValorMejora / 100.0f;
			break;

		case ETipoEstadisticaGema::RegeneracionEnergia:
			BonoRegenEnergiaPorcentaje += Gema.ValorMejora / 100.0f;
			break;

		// ... agregar los demás casos del Enum ...
		default:
			break;
		}
	}

	// 3. Aplicar los bonos porcentuales al final (para que matemáticamente sea correcto)
	//lo dejo mal para tener las dos opciones
	LifeMax      += FMath::RoundToInt(LifeMax * BonoVidaPorcentaje);
	Attack       += FMath::RoundToInt(Attack * BonoAtaquePorcentaje);
	Defense      += FMath::RoundToInt(Defense * BonoDefensaPorcentaje);
	Velocity     += Velocity * BonoVelocidadPorcentaje;
	RegenVida    += RegenVida * BonoRegenVidaPorcentaje;
	RegenEnergia += RegenEnergia * BonoRegenEnergiaPorcentaje;

	// Opcional: Asegurarse de que la Vida actual no supere la nueva Vida Máxima
	if (Life > LifeMax)
	{
		Life = LifeMax;
	}

	// Al final de la función, armamos el struct para la UI
	FEstadisticasPersonaje StatsParaUI;
	StatsParaUI.VidaMaxima = LifeMax;
	StatsParaUI.Ataque = Attack;
	StatsParaUI.Defensa = Defense;
	StatsParaUI.Velocidad = Velocity;
	StatsParaUI.Nivel = Level;
	StatsParaUI.EnergiaMaxima = EnergyMax;
	StatsParaUI.VelocidadRegeneracion = velocidadEnergyReg;
	StatsParaUI.RegenVida = RegenVida;
	StatsParaUI.RegenEnergia = RegenEnergia;

	// Disparamos el único delegado
	OnAtributosActualizados.Broadcast(StatsParaUI);
}

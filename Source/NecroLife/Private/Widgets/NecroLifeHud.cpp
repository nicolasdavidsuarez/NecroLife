// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NecroLifeHud.h"

#include "Components/AttributeComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UHealthComponent.h"

void UNecroLifeHud::NativeConstruct()
{
	Super::NativeConstruct();
	// 1. Obtenemos el Pawn (tu BP_Character)
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return;

	// 2. Buscamos el componente de salud (usa el nombre exacto de tu clase en C++)
	UUHealthComponent* HealthComp = PlayerPawn->FindComponentByClass<UUHealthComponent>();

	if (HealthComp)
	{
		// 3. ¡EL BINDEO! 
		// Primero removemos por seguridad para no binear dos veces si el widget se reconstruye
		//HealthComp->OnHealthChanged.RemoveDynamic(this, &UNecroLifeHUD::HandleHealthChanged);
        
		// Ahora sí, agregamos el bindeo
		HealthComp->OnHealthChanged.AddDynamic(this, &UNecroLifeHud::HandleHealthChanged);
        
		// Tip: Llamá a la función una vez manualmente para inicializar la barra con la vida actual
		// HandleHealthChanged(HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
	}
	UAttributeComponent* Attributes = PlayerPawn->FindComponentByClass<UAttributeComponent>();

	if (Attributes)
	{
		// Bindeamos el evento
		Attributes->OnXPChanged.AddDynamic(this, &UNecroLifeHud::HandleXPChanged);
            
		// Llamada inicial para que la barra no empiece vacía
		// HandleXPChanged(Attributes->GetXP(), Attributes->GetNextLevelXP(), Attributes->GetLevel());
	}
}

void UNecroLifeHud::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth/MaxHealth);
	}
}

void UNecroLifeHud::HandleXPChanged(float CurrentXP, float XPToNextLevel, int32 CurrentLevel)
{
	// Esto reemplaza los nodos de división y Append del Blueprint
	if (XPBar && XPToNextLevel > 0.0f)
	{
		float Percent = CurrentXP / XPToNextLevel;
		XPBar->SetPercent(Percent);
	}

	if (LevelText)
	{
		// Reemplaza el "Append" de "Nivel: " + Valor
		FString LevelString = FString::Printf(TEXT("Nivel: %d"), CurrentLevel);
		LevelText->SetText(FText::FromString(LevelString));
		
	}
}

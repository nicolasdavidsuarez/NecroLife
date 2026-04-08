// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NecroLifeHud.h"

#include "NecroLifePlayerState.h"
#include "Components/AttributeComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/UHealthComponent.h"

void UNecroLifeHud::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return;

	// 2. Buscamos el componente de salud (usa el nombre exacto de tu clase en C++)
	UUHealthComponent* HealthComp = PlayerPawn->FindComponentByClass<UUHealthComponent>();

	if (HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &UNecroLifeHud::HandleHealthChanged);

	}
	UAttributeComponent* Attributes = PlayerPawn->FindComponentByClass<UAttributeComponent>();

	if (Attributes)
	{

		Attributes->OnXPChanged.AddDynamic(this, &UNecroLifeHud::HandleXPChanged);

	}

	
	ANecroLifePlayerState* MyPS = Cast<ANecroLifePlayerState>(GetOwningPlayerState());
	if (MyPS )
	{
			
		UInventoryComponent* Inventory= MyPS->FindComponentByClass<UInventoryComponent>();
		if (Inventory)
		{
			Inventory->OnShowItem.AddDynamic(this, &UNecroLifeHud::ActualizarInventario);
			bBindeo = true;
		}
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Green, "ActualizarInventario pero no bindeo y tiene player state");	
	
	}
	GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "ActualizarInventario pero no bindeo");	

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
	
	if (XPBar && XPToNextLevel > 0.0f)
	{
		float Percent = CurrentXP / XPToNextLevel;
		XPBar->SetPercent(Percent);
	}

	if (LevelText)
	{
		FString LevelString = FString::Printf(TEXT("Nivel: %d"), CurrentLevel);
		LevelText->SetText(FText::FromString(LevelString));
		
	}
}

void UNecroLifeHud::ActualizarInventario(const TArray<UItemData*>& ItemsRecibidos)
{

	if (!bBindeo)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "ActualizarInventario pero no bindeo");	

	}else
	{		
		BP_UpdateInventoryUI(ItemsRecibidos);	
	}
}

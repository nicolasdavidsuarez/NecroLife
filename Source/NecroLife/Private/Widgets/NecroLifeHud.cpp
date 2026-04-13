// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/NecroLifeHud.h"

#include "NecroLifeCharacter.h"
#include "NecroLifeGameState.h"
#include "NecroLifePlayerState.h"
#include "Components/AbilityComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/QuestComponent.h"
#include "Components/TextBlock.h"
#include "Components/UHealthComponent.h"

void UNecroLifeHud::NativeConstruct()
{
	Super::NativeConstruct();	

	if (UWorld* World=GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandleBind,this,&UNecroLifeHud::BindDelegate,0.5f,true);	
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

void UNecroLifeHud::ActualizarInventario(const TArray<FDatosGema>& ItemsRecibidos)
{
		BP_UpdateInventoryUI(ItemsRecibidos);	
}

void UNecroLifeHud::HandleCoolDown(int32 AbilitySlot)
{
	ANecroLifeCharacter* Player = Cast<ANecroLifeCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->SetCoolDownAbility(AbilitySlot);
		bBindeoAbility=true;
	}
}

void UNecroLifeHud::HandleOnPotionChange(int CantPosiones)
{
	if (TxtPosiones)
	{
		// Esto hace el equivalente al "Append" y al "To Text" todo en una línea
		FString TextoFormateado = FString::Printf(TEXT("Posions: %d"), CantPosiones);
        
		// Seteamos el texto en la UI (el nodo "SET Text")
		TxtPosiones->SetText(FText::FromString(TextoFormateado));
	}
}

void UNecroLifeHud::BindDelegate()
{
	if (!bBindeo)
	{
		ANecroLifePlayerState* MyPS = Cast<ANecroLifePlayerState>(GetOwningPlayerState());
		if (MyPS )
		{			
			UInventoryComponent* Inventory= MyPS->FindComponentByClass<UInventoryComponent>();
			if (Inventory)
			{
				Inventory->GemsToShow.AddDynamic(this, &UNecroLifeHud::ActualizarInventario);
				Inventory->OnPotionChange.AddDynamic(this,&UNecroLifeHud::HandleOnPotionChange);
				bBindeo = true;
			}
		}
	}
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return;
	if (!bBindeoHealth)
	{
		
		UUHealthComponent* HealthComp = PlayerPawn->FindComponentByClass<UUHealthComponent>();
		if (HealthComp)
		{
			HealthComp->OnHealthChanged.AddDynamic(this, &UNecroLifeHud::HandleHealthChanged);
			bBindeoHealth=true;
		}
	
	}
	if (!bBindeoAtribute)
	{
		UAttributeComponent* Attributes = PlayerPawn->FindComponentByClass<UAttributeComponent>();
		if (Attributes)
		{
			Attributes->OnXPChanged.AddDynamic(this, &UNecroLifeHud::HandleXPChanged);
			bBindeoAtribute=true;
		}
	}

	if (!bBindeoAbility)
	{
		UAbilityComponent* AbilityComp =Cast<UAbilityComponent>(PlayerPawn->FindComponentByClass<UAbilityComponent>());
		if (AbilityComp)
		{
			AbilityComp->AbilitySlot.AddDynamic(this,&UNecroLifeHud::HandleCoolDown);
			
			bBindeoAbility=true;
		}
	}
if (!bBindeoMisiones)
{
	if (ANecroLifeGameState* GS = Cast<ANecroLifeGameState>(GetWorld()->GetGameState()))
	{
		if (UQuestComponent* QuestComp = GS->FindComponentByClass<UQuestComponent>())
		{
			// Bindeamos el evento (asegúrate de que el nombre OnUpdateObjectiveList coincida con el tuyo)
			QuestComp->OnUpdateObjectiveList.AddDynamic(this, &UNecroLifeHud::ActualizarQuestList);
			bBindeoMisiones = true;
		}
	}
}
	

	if (bBindeoAtribute&&bBindeoHealth&&bBindeo&&bBindeoMisiones&&bBindeoAbility)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleBind);
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "cLeAr tImEr");			

	}
	//GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "tImEr");
	

}

void UNecroLifeHud::ActualizarQuestList(const TArray<FQuestUIData>& QuestUi)
{
	BP_UpdateMisionesUI(QuestUi);
}

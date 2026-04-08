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

void UNecroLifeHud::ActualizarInventario(const TArray<UItemData*>& ItemsRecibidos)
{
		BP_UpdateInventoryUI(ItemsRecibidos);	
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
				Inventory->OnShowItem.AddDynamic(this, &UNecroLifeHud::ActualizarInventario);
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

	if (bBindeoAtribute&&bBindeoHealth&&bBindeo)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandleBind);
		GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "cLeAr tImEr");			

	}
	GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Red, "tImEr");			

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "Data/QuestData.h"
#include "Types/NecroLifeTypes.h"
#include "NecroLifeHud.generated.h"

class UItemData;

/**
 * 
 */
UCLASS()
class NECROLIFE_API UNecroLifeHud : public UUserWidget
{
	GENERATED_BODY()
protected:
	// Este es el "BeginPlay" de los widgets
	virtual void NativeConstruct() override;

private:
	bool bBindeo = false;
	bool bBindeoHealth=false;
	bool bBindeoAtribute = false;
	bool bBindeoMisiones=false;
	bool bBindeoAbility = false;
    FTimerHandle TimerHandleBind;
	
	public:

	
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TxtPosiones;
	
	UFUNCTION(BlueprintCallable)
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void HandleXPChanged(float CurrentXP, float XPToNextLevel, int32 CurrentLevel);

	// Si tienes la referencia a la barra de XP bindeada:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UProgressBar* XPBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* LevelText;

	// En tu clase de UI (donde recibes el evento)
	UFUNCTION()
	void ActualizarInventario(const TArray<FDatosGema>& ItemsRecibidos);

	UFUNCTION()
	void HandleCoolDown(int32 AbilitySlot);

	UFUNCTION()
	void HandleOnPotionChange(int CantPosiones);

	UFUNCTION()
	void ActualizarGemasInSlots(const TArray<FDatosGema>& DatosGemas);
	UFUNCTION()
	void BindDelegate();

	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void BP_UpdateInventoryUI(const TArray<FDatosGema>& GemsToShow);



	UFUNCTION()
	void ActualizarQuestList(const TArray<FQuestUIData>& QuestUi);

	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void BP_UpdateMisionesUI(const TArray<FQuestUIData>& QuestUi);

	
};

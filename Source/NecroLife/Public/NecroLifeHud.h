// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NecroLifeHud.generated.h"

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
	public:
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UFUNCTION(BlueprintCallable)
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void HandleXPChanged(float CurrentXP, float XPToNextLevel, int32 CurrentLevel);

	// Si tienes la referencia a la barra de XP bindeada:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* XPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;
	
};

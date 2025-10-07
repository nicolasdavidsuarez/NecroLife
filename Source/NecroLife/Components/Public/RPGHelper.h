// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGHelper.generated.h"

/**
 * 
 */
// Forward declarations para no tener que incluir all
class UUHealthComponent;
class UAttributeComponent;
class UAbilityComponent;


UCLASS()
class NECROLIFE_API URPGHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	/* =========================
  en teoria en la BlueprintFunctionLibrary se pueden inmplementar funciones que se pueden usar
  en todas las clases. aca se van a tratar de hacer funciones genericas que compartan varios actores
  funciones de validacion de datos, 
   ========================= */
	UFUNCTION(BlueprintCallable, Category="RPG|Damage")
	static bool ApplyDamage(AActor* Target, float Amount);

	UFUNCTION(BlueprintCallable, Category="RPG|Damage")
	static bool ApplyHealing(AActor* Target, float Amount);

	UFUNCTION(BlueprintCallable, Category="RPG|Attributes")
	static bool TakeXP(AActor* Target, float Amount);

	UFUNCTION(BlueprintCallable, Category="RPG|Attributes")
	static bool TakePosion(AActor* Target);
	bool PickUpItem(AActor* Target, AActor* Item);


	/*UFUNCTION(BlueprintCallable, Category="RPG|Attributes")
	static bool XpToLevelUp(AActor* Target, float Amount);*/
};

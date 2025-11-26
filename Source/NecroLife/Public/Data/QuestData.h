// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

/**
 * 
 */
// Tipos de objetivos posibles
UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
	TalkToNPC       UMETA(DisplayName = "Hablar con NPC"),
	ReachLocation   UMETA(DisplayName = "Llegar a Lugar"),
	CollectItem     UMETA(DisplayName = "Recolectar Item"),
	KillEnemies     UMETA(DisplayName = "Eliminar Enemigos"),
	InteractWith    UMETA(DisplayName = "Interactuar (Antorcha/Palanca)")
};

// La estructura de un paso individual (Un "Stage" de la misión)
// todo ver si se ve en el editor!!
USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	// Texto que sale en el HUD: "Busca bencina por el cementerio"
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
	
//Fase de la mision, todas las fases que se pueden hacer en paralelo tienen el mismo numero de fase
	//Ej: se puede matar enemigos mientras se busca la bencina, y no se pasa de objetivo hasta que todos esten
	//completo, si es lineal se le asigna un numero a cada uno.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Stage;

	// El tipo de lógica a chequear
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestObjectiveType Type;

	// ID CLAVE: ¿A quién mato? ¿Qué item busco? ¿Con quién hablo?
	// Usamos GameplayTags (Recomendado) o FName para identificar al Actor/Item
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag TargetID; 

	// Cantidad necesaria (ej: Matar 5, Juntar 1)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AmountRequired = 1;

	// (Opcional) ¿Necesita un item previo? Ej: Tener bencina para prender antorcha
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag PrerequisiteItemID;

	//(Opcional) Mas facil guardar aca que termino con el objetivo
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bObjetivoCumplido=false;
};

UCLASS()
class NECROLIFE_API UQuestData : public UDataAsset
{
	GENERATED_BODY()
	public:
	// Nombre o de la Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FName QuestName;
	// Descripcion de la Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FText Description;
	// Icono para el UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* QuestIcon;
	
	//  Una lista ordenada de pasos
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objectives")
	TArray<FQuestObjective> Objectives;
	//guardo aca si termino la quest(mas facil para la logica)
	bool bQuestDone=false;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/QuestData.h"
#include "QuestComponent.generated.h"

USTRUCT(BlueprintType)
struct FActiveQuestData
{
	GENERATED_BODY()
	// 1. ¿Qué misión es? (La referencia al "Menú", solo lectura)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestData* QuestDataAsset=nullptr;

	// 2. ¿Por qué fase va? (El estado variable)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStage = 0;

	// 3. Contadores específicos (Ej: "lobo: 2", "saqueador: 1")
	// Usamos un Map: La clave es el ID del objetivo (Tag) y el valor es la cantidad actual.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, int32> ObjectiveProgress;
    
	// Un constructor simple para inicializar
	FActiveQuestData()
	{
		QuestDataAsset = nullptr;
		CurrentStage = 0;
	}
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();	


	//Agregar quest
	UFUNCTION(BlueprintCallable, Category="Quests")
	void AddQuest(UQuestData* QuestData);

	//actualizar quests activas
	void ActualizarQuests();
	
	// Función para actualizar progreso (cuando matas algo)
	UFUNCTION(BlueprintCallable, Category="Quests")
	void UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount);
	void CheckObjetivoComplete(FActiveQuestData& QuestData, FGameplayTag ObjectiveID, int32 Amount);
	//FString Lista de tareas (ya editada)
	
	//void ingresar evento

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UQuestData*> Quest;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	
private:
	
	TArray<FQuestObjective> ObjetivosActuales;
	UQuestData* CurrentQuest=nullptr;
	TArray<FActiveQuestData> ActiveQuest;
	bool bAsQuest;
		
};

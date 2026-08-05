// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/QuestData.h"
#include "Net/UnrealNetwork.h"
#include "QuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateObjetiveList, const TArray<FQuestUIData>&, ItemsToMisionList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAdded, UQuestData*, Quest);





USTRUCT(BlueprintType)
struct FProgresoObjetivo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag TargetID;

	UPROPERTY(BlueprintReadWrite)
	int32 CantidadActual = 0;
};

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
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TMap<FGameplayTag, int32> ObjectiveProgress;
	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	TArray<FProgresoObjetivo> ObjectiveProgress;
	

	//TSet: Lista de grupos de elementos unicos. no admite duplicados. no se ordena.
	//aca tendria que administrar las misiones para no escribir el UQuestData
	UPROPERTY(BlueprintReadWrite, Category = "Quest")
	TArray<FGameplayTag> ObjetivosCompletados;
	//TSet<FGameplayTag> ObjetivosCompletados;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDone;

	/*bool operator==(const FActiveQuestData& Other) const
	{
			return QuestDataAsset == Other.QuestDataAsset;
	}*/
	
	// Un constructor simple para inicializar
	FActiveQuestData()
	{
		QuestDataAsset = nullptr;
		CurrentStage = 0;
		bIsDone = false;
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Quests")
	TArray<FName> QuestsCompletadasNombre;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI | Quests")
	TArray<FName> QuestsActivasNombre;

	
	UPROPERTY(BlueprintAssignable, Category = "Quests|UI")
	FOnUpdateObjetiveList OnUpdateObjectiveList;
	
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnQuestAdded OnQuestAdded;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, UQuestData*, Quest);
	
	UPROPERTY(BlueprintAssignable, Category = "Quests")
	FOnQuestCompleted OnQuestCompleted;

	// Función para actualizar listas de completadas y listas
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void ActualizarListasQuests();
	
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool haveQuests();
	
	UFUNCTION(BlueprintCallable, Category = "Quests")
	bool hasQuest(UQuestData* QuestToFind);
	
	UFUNCTION(BlueprintCallable)
	bool IsObjectiveComplete(FGameplayTag ObjectiveID);
	bool IsStageComplete(UQuestData* QuestData, int32 Stage);
	bool IsStageComplete(int32 Stage);

	// Función para actualizar progreso (cuando matas algo)
	UFUNCTION(BlueprintCallable, Category="Quests")
	bool UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount);
	
	void CheckObjetivoComplete(FActiveQuestData& QuestToCheck, FGameplayTag ObjectiveID);
	//FString Lista de tareas (ya editada)
	
	//void ingresar evento

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(ReplicatedUsing = OnRep_MisionesActualizadas)
	TArray<UQuestData*> Quests;
	
	UFUNCTION()
	void OnRep_MisionesActualizadas();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(ReplicatedUsing = OnRep_ActiveQuests)
	TArray<FActiveQuestData> ActiveQuests;
	
	bool bAsQuest;

	UFUNCTION(BlueprintCallable, Category = "Quests")
	void OnRep_ActiveQuests();	
	
			
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"

#include <string>

#include "Data/QuestData.h"
#include "Engine/NetworkObjectList.h"


// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	Quests.Empty();
}

void UQuestComponent::OnRep_ActiveQuests()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("CLIENTE: Recibí la actualización de misiones por red!"));
	ActualizarQuests();
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		// ...
}

void UQuestComponent::AddQuest(UQuestData* QuestData)
{

	if (!GetOwner()->HasAuthority()) return;
	
//La lista quets, tiene todas las que han sido cargadas, pueden estar completadas o no
    if (Quests.Contains(QuestData))
	{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString::Printf(TEXT("Ya contenia el data Asset")));	
	
	}else{
	Quests.Add(QuestData);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
    	FString::Printf(TEXT("Se agrego el data Asset. %s"), *QuestData->Description.ToString()));
		FActiveQuestData tempQuest;
		tempQuest.CurrentStage=0;
		tempQuest.QuestDataAsset=QuestData;
		tempQuest.bIsDone=false;
		tempQuest.ObjectiveProgress.Empty();
		for (int i=0;i<QuestData->Objectives.Num();i++)//inicializacion del los objetivos
		{
			//GEngine->AddOnScreenDebugMessage(i+1, 50.f, FColor::Yellow,
		//QuestData->Objectives[i].Description.ToString());
			tempQuest.ObjectiveProgress.Add(QuestData->Objectives[i].TargetID,0);
		}
		ActiveQuests.Add(tempQuest);
	ActualizarQuests();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("cantidad de quest: %d"),	Quests.Num()));
		//CurrentQuest = QuestData;		
}

void UQuestComponent::ActualizarQuests()
{
	//if (Quests.Num() == 0) return;

	// 1. Arreglo principal que enviaremos a la UI
	TArray<FQuestUIData> ListaQuestsParaUI;

	for (const auto& ActiveQuestTemp : ActiveQuests)  
	{
		// 2. Creamos el contenedor para ESTA misión específica
		FQuestUIData NuevaQuestUI;
		NuevaQuestUI.QuestName = FText::FromName(ActiveQuestTemp.QuestDataAsset->QuestName);

		// 3. Buscamos los objetivos de la etapa actual
		for (int i = 0; i < ActiveQuestTemp.QuestDataAsset->Objectives.Num(); i++)
		{
			if (ActiveQuestTemp.QuestDataAsset->Objectives[i].Stage == ActiveQuestTemp.CurrentStage)
			{
				FQuestObjectiveListEntry NuevoObjetivo;
				NuevoObjetivo.EntryText = ActiveQuestTemp.QuestDataAsset->Objectives[i].Description;
				NuevoObjetivo.img = nullptr; 
				NuevoObjetivo.bIsDone = ActiveQuestTemp.ObjetivosCompletados.Contains(ActiveQuestTemp.QuestDataAsset->Objectives[i].TargetID);

				// Agregamos el objetivo a la lista de esta misión
				NuevaQuestUI.Objectives.Add(NuevoObjetivo);
			}
		}
       
		// 4. Si la misión tiene objetivos en esta etapa, la agregamos a la lista final
		if (NuevaQuestUI.Objectives.Num() > 0)
		{
			ListaQuestsParaUI.Add(NuevaQuestUI);
		}
	}
    
	// 5. Disparamos el delegado con toda la información empaquetada
	OnUpdateObjectiveList.Broadcast(ListaQuestsParaUI);
}


void UQuestComponent::ActualizarListasQuests()
{
	// Para la ui mas que nada
	QuestsActivasNombre.Empty();
	QuestsCompletadasNombre.Empty();

	// 1. Creamos el arreglo que vamos a enviar por el delegado
	TArray<FQuestObjectiveListEntry> ListaMisionesUI;

	for (const UQuestData* Quest : Quests)
	{
		if (Quest)
		{
			
			if (Quest->bQuestDone) 
			{
				QuestsCompletadasNombre.Add(Quest->QuestName);
			} else {
				QuestsActivasNombre.Add(Quest->QuestName);
			}

			// 2. Creamos una entrada para nuestra estructura y la llenamos
			FQuestObjectiveListEntry NuevaEntrada;
			NuevaEntrada.EntryText = FText::FromName(Quest->QuestName); // Asumiendo que QuestName es FText
			NuevaEntrada.bIsDone = Quest->bQuestDone;
         
			NuevaEntrada.img = Quest->QuestIcon; 

			// 3. Añadimos la entrada al arreglo
			ListaMisionesUI.Add(NuevaEntrada);
		}
	}
    
	// 4. Disparamos el delegado. La UI recibirá este arreglo.
	//OnUpdateObjectiveList.Broadcast(ListaMisionesUI);
}

bool UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{

	// Si NO somos el servidor (Autoridad), no hacemos nada.
	if (!GetOwner()->HasAuthority()) return false; 

	
	if (!ObjectiveID.IsValid()) return false;

	bool bHasUpdatedProgress = false;
    
	// Usamos referencia (&) para poder modificar el struct dentro del array
	for (FActiveQuestData& ActiveQuestData : ActiveQuests)
	{
		// 1. Verificamos que tengamos un DataAsset válido para esta misión activa
		if (!ActiveQuestData.QuestDataAsset) continue;

		bool bIsObjectiveInCurrentStage = false;

		// 2. BUSCAMOS EL OBJETIVO EN EL DATA ASSET
		// Recorremos la plantilla de la misión para ver en qué Stage está configurado este ObjectiveID
		for (const FQuestObjective& ObjectiveTemplate : ActiveQuestData.QuestDataAsset->Objectives)
		{
			if (ObjectiveTemplate.TargetID == ObjectiveID)
			{
				// Encontramos el objetivo. ¿Su fase corresponde a la fase actual de la misión?
				if (ObjectiveTemplate.Stage == ActiveQuestData.CurrentStage)
				{
					bIsObjectiveInCurrentStage = true;
				}
				break; // Ya encontramos el ID, dejamos de buscar en el Data Asset
			}
		}

		// 3. VALIDACIÓN: Si no pertenece a la etapa actual, ignoramos esta misión y pasamos a la siguiente
		if (!bIsObjectiveInCurrentStage)
		{
			continue; 
		}

		// 4. ACTUALIZAMOS EL PROGRESO (como ya lo tenías)
		int32* CurrentProgress = ActiveQuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentProgress)
		{
			*CurrentProgress += Amount; 
            
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Progreso: %d"), *CurrentProgress)); 
          
			// Chequear si se completó el objetivo pasándole la data actualizada
			CheckObjetivoComplete(ActiveQuestData, ObjectiveID);
          
			bHasUpdatedProgress = true;
		}   
	}

	return bHasUpdatedProgress;
}
// En UpdateQuestProgress
/*bool UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{
	bool progress = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
								FString::Printf(TEXT("Active Quests: %s"),*ObjectiveID.ToString())); 
	//esta es la funcion que se llama en blueprints cuando interactua en el juego
	if (!ObjectiveID.IsValid()) return progress;
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString::Printf(TEXT("Se agrego el data Asset. %s"), *QuestData->Description.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
							FString::Printf(TEXT("Active Quests: %s"),*ObjectiveID.ToString())); 
	// Usamos referencia (&) para poder modificar el struct dentro del array
	for (FActiveQuestData& ActiveQuestData : ActiveQuests)
	{
		// 1. Buscamos si esta quest tiene ese objetivo
		// Asumo que ObjectiveProgress guarda: <TagObjetivo, CantidadActual>
		int32* CurrentProgress = ActiveQuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentProgress)
		{
			// 2. CORRECCIÓN PUNTERO: Desreferenciar con *
			*CurrentProgress += Amount; 
            
			// Log para debug
			//UE_LOG(LogTemp, Warning, TEXT("Progreso actualizado: %d"), *CurrentProgress);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
						FString::FromInt(*CurrentProgress)); 
			// 3. Chequear si se completó (Pasamos la ActiveQuestData actual)
			CheckObjetivoComplete(ActiveQuestData, ObjectiveID);
			progress = true;
		}   
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
						FString("Entro a updatear"));
	return progress;
}*/


void UQuestComponent::CheckObjetivoComplete(FActiveQuestData& QuestToCheck, FGameplayTag ObjectiveID)
{
	// 1. Buscar cuánto necesitamos LEYENDO DEL DATA ASSET (que es estático)
	int32 RequiredAmount = 0;
	bool bFoundObjective = false;

	for (const auto& Objective : QuestToCheck.QuestDataAsset->Objectives)
	{
		if (Objective.TargetID == ObjectiveID)
		{
			RequiredAmount = Objective.AmountRequired;
			bFoundObjective = true;
			break;
		}
	}

	if (!bFoundObjective) return;

	// 2. Obtener cuanto tenemos actualmente
	int32* FoundPtr = QuestToCheck.ObjectiveProgress.Find(ObjectiveID);
	int32 CurrentAmount = FoundPtr ? *FoundPtr : 0;
	// 3. Comparar
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("Cantidad/Necesario: %d / %d"),	CurrentAmount, RequiredAmount));
		
	if (CurrentAmount >= RequiredAmount)
	{
		// 4. CORRECCIÓN DATA ASSET: Guardar estado en la estructura local, no en el asset
		// Asegúrate de agregar un bool bIsCompleted en tu struct FActiveQuestData
		 //QuestToCheck.bIsDone=true;//esto esta mal porque solo debe actualizar el objetivo


		for (auto& Objective : QuestToCheck.QuestDataAsset->Objectives)
		{
			if (Objective.TargetID == ObjectiveID)
			{
			    //Objective.bObjetivoCumplido=true;
				QuestToCheck.ObjetivosCompletados.Add(ObjectiveID);
				break;
			}
		}
   
		

			
//todo: aca deberia comprobar si hay algun objetivo que falta de la misma stage antes de avanzar
bool bFaltaObjetivo = false;
		
		for (const auto& Objective : QuestToCheck.QuestDataAsset->Objectives)
		{
			// Si el objetivo es de la etapa actual...
			if (Objective.Stage == QuestToCheck.CurrentStage)
			{
				// ...y NO está en nuestra lista de completados
				if (!QuestToCheck.ObjetivosCompletados.Contains(Objective.TargetID))
				{
					bFaltaObjetivo = true;
					break;
				}
			}
		}
if (!bFaltaObjetivo)
{
	QuestToCheck.CurrentStage++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
FString::Printf(TEXT("¡Objetivo Completado! Nueva etapa: %d"), QuestToCheck.CurrentStage));
}
		
		
		// Aquí podrías llamar a una función que verifique si TODOS los objetivos de la quest están listos
	}
	ActualizarQuests();
}

//esto no se para que es pero es obligatorio a todas las variables que se repliquen
void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Registramos ActiveQuests para que se envíe a todos los clientes
	DOREPLIFETIME(UQuestComponent, ActiveQuests);
	DOREPLIFETIME(UQuestComponent,Quests);
}






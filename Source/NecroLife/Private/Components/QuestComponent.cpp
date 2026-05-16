// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"

#include <string>

#include "Data/QuestData.h"
#include "Engine/NetworkObjectList.h"
#include "Net/UnrealNetwork.h" 


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
	FActiveQuestData tempQuest;
	for (int i = 0; i < QuestData->Objectives.Num(); i++)
	{
		FProgresoObjetivo NuevoProgreso;
		NuevoProgreso.TargetID = QuestData->Objectives[i].TargetID;
		NuevoProgreso.CantidadActual = 0;    
		tempQuest.ObjectiveProgress.Add(NuevoProgreso); // Ahora es un TArray
	}
	
//La lista quets, tiene todas las que han sido cargadas, pueden estar completadas o no
    if (Quests.Contains(QuestData))
	{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	FString::Printf(TEXT("Ya contenia el data Asset")));	
	
	}else{
	Quests.Add(QuestData);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
    	FString::Printf(TEXT("Se agrego el data Asset. %s"), *QuestData->Description.ToString()));
		//FActiveQuestData tempQuest;
		tempQuest.CurrentStage=0;
		tempQuest.QuestDataAsset=QuestData;
		tempQuest.bIsDone=false;
		tempQuest.ObjectiveProgress.Empty();
		for (int i=0;i<QuestData->Objectives.Num();i++)//inicializacion del los objetivos
		{
			//GEngine->AddOnScreenDebugMessage(i+1, 50.f, FColor::Yellow,
		//QuestData->Objectives[i].Description.ToString());
			//tempQuest.ObjectiveProgress.Add(QuestData->Objectives[i].TargetID,0);

			// 1. Creamos la "caja" (la estructura)
			FProgresoObjetivo NuevoProgreso;    
			// 2. La rellenamos con el ID y el progreso inicial en 0
			NuevoProgreso.TargetID = QuestData->Objectives[i].TargetID;
			NuevoProgreso.CantidadActual = 0;    
			// 3. Metemos la caja completa al Array
			tempQuest.ObjectiveProgress.Add(NuevoProgreso);
		}
		ActiveQuests.Add(tempQuest);
		//OnRep_MisionesActualizadas();
	ActualizarQuests();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("cantidad de quest: %d"),	Quests.Num()));
		//CurrentQuest = QuestData;		
}

void UQuestComponent::OnRep_MisionesActualizadas()
{
	ActualizarQuests();
}

void UQuestComponent::ActualizarQuests()
{
	
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
	//OnRep_MisionesActualizadas();
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
    
}

bool UQuestComponent::haveQuests()
{
	if (!Quests.IsEmpty())
	{
		return true;	
	}
	return false;
}

bool UQuestComponent::UpdateQuestProgress(FGameplayTag ObjectiveID, int32 Amount)
{

	if (!GetOwner()->HasAuthority() || !ObjectiveID.IsValid()) return false;

	bool bHasUpdatedProgress = false;
    
	// Usamos un bucle clásico con índice para asegurar que Unreal detecte la modificación
	for (int32 i = 0; i < ActiveQuests.Num(); i++)
	{
		if (!ActiveQuests[i].QuestDataAsset) continue;

		bool bIsObjectiveInCurrentStage = false;

		for (const FQuestObjective& ObjectiveTemplate : ActiveQuests[i].QuestDataAsset->Objectives)
		{
			if (ObjectiveTemplate.TargetID == ObjectiveID && ObjectiveTemplate.Stage == ActiveQuests[i].CurrentStage)
			{
				bIsObjectiveInCurrentStage = true;
				break; 
			}
		}

		if (!bIsObjectiveInCurrentStage) continue; 

		// Buscar el objetivo en nuestro nuevo TArray de progreso
		for (int32 j = 0; j < ActiveQuests[i].ObjectiveProgress.Num(); j++)
		{
			if (ActiveQuests[i].ObjectiveProgress[j].TargetID == ObjectiveID)
			{
				// Aumentamos la cantidad
				ActiveQuests[i].ObjectiveProgress[j].CantidadActual += Amount; 
                
				// Evaluamos si se completó
				CheckObjetivoComplete(ActiveQuests[i], ObjectiveID);
                
				bHasUpdatedProgress = true;
				break; // Salimos del bucle interno
			}
		}
	}

	if (bHasUpdatedProgress)
	{
		// TRUCO DE RED: Forzamos al servidor a actualizar a los clientes
		// Si el cliente no recibe el OnRep, esta línea lo obliga a revisar el componente.
		GetOwner()->ForceNetUpdate(); 
	}

	return bHasUpdatedProgress;
	
	// Si NO somos el servidor (Autoridad), no hacemos nada.
	/*if (!GetOwner()->HasAuthority()) return false; 
	
	if (!ObjectiveID.IsValid()) return false;

	bool bHasUpdatedProgress = false;
    
	
	for (FActiveQuestData& ActiveQuestData : ActiveQuests)
	{
		
		if (!ActiveQuestData.QuestDataAsset) continue;

		bool bIsObjectiveInCurrentStage = false;

			for (const FQuestObjective& ObjectiveTemplate : ActiveQuestData.QuestDataAsset->Objectives)
		{
			if (ObjectiveTemplate.TargetID == ObjectiveID)
			{
				if (ObjectiveTemplate.Stage == ActiveQuestData.CurrentStage)
				{
					bIsObjectiveInCurrentStage = true;
				}
				break; // Ya encontramos, no es necesario recorrer todo
			}
		}

		if (!bIsObjectiveInCurrentStage)
		{
			continue; 
		}
		int32* CurrentProgress = ActiveQuestData.ObjectiveProgress.Find(ObjectiveID);

		if (CurrentProgress)
		{
			*CurrentProgress += Amount; 
            
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Progreso: %d"), *CurrentProgress)); 
          
			CheckObjetivoComplete(ActiveQuestData, ObjectiveID);
          
			bHasUpdatedProgress = true;
		}   
	}
	return bHasUpdatedProgress;*/
}


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
	/*int32* FoundPtr = QuestToCheck.ObjectiveProgress.Find(ObjectiveID);
	int32 CurrentAmount = FoundPtr ? *FoundPtr : 0;*/
	int32 CurrentAmount = 0;
	for (const auto& Progreso : QuestToCheck.ObjectiveProgress)
	{
		if (Progreso.TargetID == ObjectiveID)
		{
			CurrentAmount = Progreso.CantidadActual;
			break;
		}
	}
	
	// 3. Comparar
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
		FString::Printf(TEXT("Cantidad/Necesario: %d / %d"),	CurrentAmount, RequiredAmount));
		
	if (CurrentAmount >= RequiredAmount)
	{
	

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
	DOREPLIFETIME(UQuestComponent,ActiveQuests);
	DOREPLIFETIME(UQuestComponent,Quests);
}







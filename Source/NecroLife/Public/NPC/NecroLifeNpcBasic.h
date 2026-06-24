// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestData.h"
#include "GameFramework/Character.h"
#include "Interface/NecroLifeInterface.h"
#include "Data/NecroLifeDialogData.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NecroLifeNpcBasic.generated.h"

class USphereComponent;
class ANecroLifeCharacter;
//struct FGameplayTag;

USTRUCT(BlueprintType)
struct FQuestTeleportEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UQuestData* Mision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Destino = nullptr;
};


UCLASS()
class NECROLIFE_API ANecroLifeNpcBasic : public ACharacter, public INecroLifeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANecroLifeNpcBasic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsTalking = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsAttacking = false;
	
	FTimerHandle TalkingTimerHandle;
	
	//para mover durante las misiones
	// La misión específica que activa el movimiento de este NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuestTeleportEntry> TeleportsPorMision;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TeleportarA(FVector Location, FRotator Rotation);
	
	UFUNCTION()
	void OnQuestAgregada(UQuestData* Quest);

	void TeleportarA(AActor* Destino);
	void EjecutarTeleportDespuesDeEfecto();


	// La referencia al Niagara, asignable desde el BP del NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* TeleportFX = nullptr;

	// Datos pendientes del teleport (los guardamos para usarlos después del timer)
	FVector TeleportDestino;
	FRotator TeleportRotacion;

	// Funciones


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UQuestData*> Quests;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag NpcTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UQuestData* QuestActual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* NPCMesh;

	// Función que llamará el Widget (UI) cuando el jugador haga clic en una respuesta
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ReceivePlayerResponse();

protected:
	// Guarda el tiempo de la última interacción
	float LastInteractTime = 0.0f;

	// Cuánto tiempo hay que esperar entre interacciones (en segundos)
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	float InteractCooldown = 0.5f;

public:
	// Esta es la llave que traba el diálogo
	UPROPERTY(Replicated)
	bool bIsWaitingForResponse = false;
	

protected:
	// El archivo que contiene el guion de este NPC
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	UNecroLifeDialogData* DialogueData;

	// Para saber por qué línea del diálogo vamos
	//int32 CurrentDialogIndex = 0;
	//int32 CurrentQuestIndex = 0;
	int32 LastQuestIndex = 0;
	int32 LastDialogIndex = 0;
//net
protected:
	// Le avisamos al sistema de red que llame a 'OnRep_CurrentQuestIndex' al recibir el cambio
	UPROPERTY(ReplicatedUsing = OnRep_CurrentQuestIndex)
	int32 CurrentQuestIndex=0;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentQuestIndex)
	int32 CurrentDialogIndex = 0;

	// Esta función se ejecutará en los Clientes automáticamente
	UFUNCTION()
	void OnRep_CurrentQuestIndex();

	
	

public:
	// Implementamos la función de la interfaz que configuramos antes
	virtual void OnInteract_Implementation(AActor* Interactor) override;

	UFUNCTION(BLueprintCallable, Category="Interact")
	void CancelAddQuest();

	UFUNCTION(BLueprintCallable, Category="Interact")
	void NextAddQuest();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		
};

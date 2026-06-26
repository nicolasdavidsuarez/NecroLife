// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NecroLifeNpcBasic.h"

#include "AIController.h"
#include "NecroLifeCharacter.h"
#include "NecroLifeGameState.h"
#include "Components/CapsuleComponent.h"
#include "Components/QuestComponent.h"

#include "Components/SphereComponent.h"


class AAIController;
// Sets default values
ANecroLifeNpcBasic::ANecroLifeNpcBasic()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Crear el componente de esfera
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    
	// Configurarlo como raíz o atarlo a la raíz
	RootComponent = SphereCollision;

	// 2. Crear el Mesh
	NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));

	 	// Atamos el Mesh a la Raíz para que se mueva con el Actor
	NPCMesh->SetupAttachment(RootComponent);

	// Configuración básica de colisión
	SphereCollision->SetSphereRadius(150,true);
	SphereCollision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ANecroLifeNpcBasic::BeginPlay()
{
	Super::BeginPlay();
	// AQUÍ conectas la esfera con TUS funciones
	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ANecroLifeNpcBasic::OnOverlapBegin);
		SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ANecroLifeNpcBasic::OnOverlapEnd);
	}
	
	if (AGameStateBase* GS = GetWorld()->GetGameState())
	{
		if (UQuestComponent* QC = GS->FindComponentByClass<UQuestComponent>())
		{
			QC->OnQuestAdded.AddDynamic(this, &ANecroLifeNpcBasic::OnQuestAgregada);
		}
	}
}

// Called every frame
void ANecroLifeNpcBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANecroLifeNpcBasic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANecroLifeNpcBasic::ReceivePlayerResponse()
{
	// El jugador ya respondió, quitamos el bloqueo
	bIsWaitingForResponse = false;
}

void ANecroLifeNpcBasic::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ANecroLifeCharacter* Character = Cast<ANecroLifeCharacter>(OtherActor);
		if (Character)
		{
			Character->LookAt(GetActorLocation());
		}
	}
}

void ANecroLifeNpcBasic::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANecroLifeCharacter* Character = Cast<ANecroLifeCharacter>(OtherActor);
	if (Character)
	{
		// El personaje se fue, dejamos de mirar
		//Character->StopLooking();
	}
}


void ANecroLifeNpcBasic::OnRep_CurrentQuestIndex()
{
	///ejecutar aca que pasa en el cliente que no esta hablando con el character npc
	
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, TEXT("mostrar si no es el que habla con npc"));
}


void ANecroLifeNpcBasic::OnInteract_Implementation(AActor* Interactor)
{
    if (bIsWaitingForResponse) return;

    ANecroLifeCharacter* MyCharacter = Cast<ANecroLifeCharacter>(Interactor);
    if (!MyCharacter) return;

    // Cooldown
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastInteractTime < InteractCooldown) return;
    LastInteractTime = CurrentTime;

    if (!DialogueData || DialogueData->DialogLines.Num() == 0) return;

    // Clamp defensivo
    CurrentDialogIndex = FMath::Clamp(CurrentDialogIndex, 0, DialogueData->DialogLines.Num() - 1);

    FDialogLine CurrentLine = DialogueData->DialogLines[CurrentDialogIndex];
	// Después del clamp defensivo
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
		FString::Printf(TEXT("Index al entrar: %d | Total lineas: %d"),
		CurrentDialogIndex, DialogueData->DialogLines.Num()));

    // --- Caso 1: línea que requiere objetivo completado ---
	if (CurrentLine.bRequiresPreviousObjective)
	{
		ANecroLifeGameState* GS = GetWorld()->GetGameState<ANecroLifeGameState>();
		if (!GS || !GS->QuestComponent) return;

		if(GS->QuestComponent->UpdateQuestProgress(FGameplayTag::RequestGameplayTag("Necro_Tags.Npc.ViejoSabio"), 1))
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow,TEXT("viva peron"));
			//aca deberia spawnear el premio por la mision
			MyCharacter->ShowDialogue(CurrentLine);
			bIsTalking = true;
			GetWorldTimerManager().SetTimer(
				TalkingTimerHandle,
				[this]() { bIsTalking = false; },
				2.5f, // segundos que dura el diálogo
				false
			);
			MyCharacter->Server_SpawnReward();
			//MyCharacter->Bp_SpawnReward();
		}
	
		bool bObjetivoCompletado = GS->QuestComponent->IsStageComplete(CurrentLine.RequiredStage);
		

		if (!bObjetivoCompletado)
		{
			// Objetivo no completado, mostramos la línea de bloqueo y no avanzamos
			MyCharacter->SetUIState(true);
			MyCharacter->ShowDialogue(CurrentLine);
			bIsTalking = true;
			GetWorldTimerManager().SetTimer(
				TalkingTimerHandle,
				[this]() { bIsTalking = false; },
				2.5f, // segundos que dura el diálogo
				false
			);
			return;
		}else
		{
				CurrentDialogIndex++;
				if (CurrentDialogIndex >= DialogueData->DialogLines.Num()) return;
				CurrentLine = DialogueData->DialogLines[CurrentDialogIndex];
				MyCharacter->SetUIState(true);
				MyCharacter->ShowDialogue(CurrentLine);
			bIsTalking = true;
			GetWorldTimerManager().SetTimer(
				TalkingTimerHandle,
				[this]() { bIsTalking = false; },
				2.5f, // segundos que dura el diálogo
				false
			);
    
				if (CurrentDialogIndex < DialogueData->DialogLines.Num() - 1)
					CurrentDialogIndex++;
    
				return;
			
		}
	
	}

	// Flujo normal (aplica también después del bloque de arriba)
	MyCharacter->SetUIState(true);
	MyCharacter->ShowDialogue(CurrentLine);
	bIsTalking = true;
	GetWorldTimerManager().SetTimer(
		TalkingTimerHandle,
		[this]() { bIsTalking = false; },
		2.5f, // segundos que dura el diálogo
		false
	);
	
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
	FString::Printf(TEXT("Flujo normal - Index: %d | Linea: %s"),
	CurrentDialogIndex,
	*DialogueData->DialogLines[CurrentDialogIndex].DialogueText.ToString()));

	if (CurrentLine.bIsMissionChoice)
	{
		bIsWaitingForResponse = true;
		QuestActual = Quests[CurrentQuestIndex];
	
		return;
	}

	if (CurrentDialogIndex < DialogueData->DialogLines.Num() - 1)
	{
		CurrentDialogIndex++;
	}

	if (CurrentLine.SpeakerAnim && GetMesh())
	{
		PlayAnimMontage(CurrentLine.SpeakerAnim);
	}
	
	
}

void ANecroLifeNpcBasic::CancelAddQuest()
{
	//CurrentDialogIndex = LastDialogIndex;
	bIsWaitingForResponse = false;
}

void ANecroLifeNpcBasic::NextAddQuest()
{
	LastDialogIndex=CurrentDialogIndex;
	CurrentQuestIndex++;
	bIsWaitingForResponse = false;
	if (CurrentDialogIndex < DialogueData->DialogLines.Num() - 1)
	{
		CurrentDialogIndex++;
	}
}

//para mover por las misiones
void ANecroLifeNpcBasic::OnQuestAgregada(UQuestData* Quest)
{
	if (!HasAuthority() || !Quest) return;

	for (const FQuestTeleportEntry& Entry : TeleportsPorMision)
	{
		if (Entry.Mision == Quest && Entry.Destino)
		{
			if (AAIController* AIC = Cast<AAIController>(GetController()))
				AIC->StopMovement();
			FVector LocationAjustada = Entry.Destino->GetActorLocation() + FVector(200.0f, 0.f, 90.f);
			Multicast_TeleportarA(
				LocationAjustada,
				Entry.Destino->GetActorRotation()
			);
			return; // una misión activa un solo teleport
		}
	}
}


void ANecroLifeNpcBasic::TeleportarA(AActor* Destino)
{
	if (!Destino) return;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
		AIC->StopMovement();

	SetActorLocationAndRotation(
		Destino->GetActorLocation(),
		Destino->GetActorRotation(),
		false, nullptr, ETeleportType::TeleportPhysics
	);
}

void ANecroLifeNpcBasic::Multicast_TeleportarA_Implementation(FVector Location, FRotator Rotation)
{
	TeleportDestino = Location;
	TeleportRotacion = Rotation;

	// Spawnear efecto en la posición actual del NPC
	if (TeleportFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TeleportFX,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	// Ocultar el mesh del NPC
	if (USkeletalMeshComponent* NpcMesh = GetMesh())
	{
		NpcMesh->SetVisibility(false);
	}
		

	// Timer: después de 1.5s ejecutar el teleport real
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ANecroLifeNpcBasic::EjecutarTeleportDespuesDeEfecto,
		1.5f,
		false
	);
}

void ANecroLifeNpcBasic::EjecutarTeleportDespuesDeEfecto()
{
	FVector LocationAjustada = TeleportDestino;
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
		LocationAjustada.Z += Capsule->GetScaledCapsuleHalfHeight()/2;

	SetActorLocationAndRotation(
		LocationAjustada, TeleportRotacion,
		false, nullptr, ETeleportType::TeleportPhysics
	);

	// Volver a mostrar el mesh
	if (USkeletalMeshComponent* NpcMesh = GetMesh())
		NpcMesh->SetVisibility(true);
}


//net
void ANecroLifeNpcBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Registramos la variable para que viaje por internet
	DOREPLIFETIME(ANecroLifeNpcBasic, CurrentQuestIndex);
	DOREPLIFETIME(ANecroLifeNpcBasic, bIsWaitingForResponse);

}

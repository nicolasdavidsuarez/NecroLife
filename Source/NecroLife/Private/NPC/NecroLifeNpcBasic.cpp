// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/NecroLifeNpcBasic.h"
#include "NecroLifeCharacter.h"
#include "Components/QuestComponent.h"

#include "Components/SphereComponent.h"


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

	// 3. ¡ESTO ES LO QUE TE FALTA! 
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
			// El personaje entró en el rango, que empiece a mirar
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


void ANecroLifeNpcBasic::OnInteract_Implementation(AActor* Interactor)
{
	//ACA se implementa la interface, muestra las lineas de dialogo que tiene el npc hasta que pregunta}
	// Si acepta la mision
	UE_LOG(LogTemp, Warning, TEXT("implementa on interact"));
	UE_LOG(LogTemp, Warning, TEXT("CurrentDialogIndex: %d"),CurrentDialogIndex);
	
	if (bIsWaitingForResponse)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Esperando que el jugador responda en la UI..."));
		return; 
	}
	
	ANecroLifeCharacter* MyCharacter = Cast<ANecroLifeCharacter>(Interactor);


	// --- 1. NUEVA VALIDACIÓN DE COOLDOWN ---
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastInteractTime < InteractCooldown)
	{
		// Si no ha pasado el tiempo suficiente, ignoramos el input
		return; 
	}
	// Actualizamos el tiempo de la última interacción válida
	LastInteractTime = CurrentTime;
	
	// 1. Verificamos si tenemos datos asignados
	if (!DialogueData || DialogueData->DialogLines.Num() == 0) return;
//CurrentDialogIndex=LastDialogIndex;
	// 2. Comprobamos si todavía quedan líneas por leer
	if (CurrentDialogIndex < DialogueData->DialogLines.Num())
	{
		//la linea siguienete updatea el Hablar con npc, pero tiene que hacerlo solo cuando sea ese tipo de mision
		
		MyCharacter->SetUIState(true);
		// Obtenemos la línea actual
		FDialogLine CurrentLine = DialogueData->DialogLines[CurrentDialogIndex];

		
		//la linea siguienete updatea el Hablar con npc, pero tiene que hacerlo solo cuando sea ese tipo de mision
		/*if (MyCharacter->QuestComponent->UpdateQuestProgress(NpcTag,1))
		{
			CurrentDialogIndex++;	
		}*/
		// --- AQUÍ VA LA COMUNICACIÓN CON LA UI ---
		// Por ahora, lo mostramos en pantalla para debuguear
		
		FString DialogMsg = FString::Printf(TEXT("%s: %s Aca si o ni tiene linea de dialogos!!!"), 
			*CurrentLine.SpeakerName.ToString(), 
			*CurrentLine.DialogueText.ToString());
		if (CurrentLine.bRequiresPreviousObjective)
		{
			// Si el jugador cumplió el objetivo y la quest se actualiza
			if (MyCharacter->QuestComponent->UpdateQuestProgress(NpcTag, 1))
			{
				// Avanzamos el índice al diálogo de "¡Gracias por ayudarme!"
				CurrentDialogIndex++;
				CurrentLine = DialogueData->DialogLines[CurrentDialogIndex];
				UE_LOG(LogTemp, Warning, TEXT("MANDA A IMPRIMIR MENSAJE LUEGO DEL UPDATEQUESTPROGRESS"));
				//MyCharacter->ShowDialogue(CurrentLine);
			
			}
			// Si UpdateQuestProgress dio false, CurrentDialogIndex no avanza, 
			// y CurrentLine sigue siendo la de "Aún no terminaste, ve a buscar eso".

			// Ahora sí, mandamos a la UI la línea (ya sea la nueva o la vieja)
			MyCharacter->ShowDialogue(CurrentLine);
		
		}
		else
		{
			// Si no requiere objetivo, mostramos y avanzamos normalmente
			MyCharacter->ShowDialogue(CurrentLine);
			CurrentDialogIndex++;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DialogMsg);

		// 3. Si tiene animación asociada, la reproducimos
		if (CurrentLine.SpeakerAnim && GetMesh())
		{
			PlayAnimMontage(CurrentLine.SpeakerAnim);
		}

		if (CurrentLine.bIsMissionChoice)
		{
			bIsWaitingForResponse = true;
	QuestActual=Quests[CurrentQuestIndex];
			if (QuestActual == nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: El elemento en el array Quests es NULO en C++"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ÉXITO: QuestActual se asignó correctamente en C++"));
			}
		}
			// 4. Incrementamos el índice para la próxima vez que el jugador apriete la "T"

		
		
	}
	else
	{
		// Se terminaron las líneas de diálogo
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			TEXT("Fin de la charla. deberia repetir el ultimo mensaje"));
      MyCharacter->SetUIState(true);
		FDialogLine CurrentLine = DialogueData->DialogLines.Last();
		//CurrentDialogIndex = DialogueData->DialogLines.Num() - 1;
		MyCharacter->ShowDialogue(CurrentLine);

		// Aquí es donde llamarías a Character->SetUIState(false) para liberar el movimiento
	}
}

void ANecroLifeNpcBasic::CancelAddQuest()
{
	CurrentDialogIndex = LastDialogIndex;
	bIsWaitingForResponse = false;
}

void ANecroLifeNpcBasic::NextAddQuest()
{
	LastDialogIndex=CurrentDialogIndex;
	CurrentQuestIndex++;
	bIsWaitingForResponse = false;
}


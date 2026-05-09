// Fill out your copyright notice in the Description page of Project Settings.
//AbilityComponent componente de habilidades, basado en GAS
//
// GAS organiza la lógica de las acciones en 3 pasos:
//   Input -> Ability -> Outcome
// - Input: la entrada del jugador (ej: tecla Q, click).
// - Ability: la lógica que valida costos, cooldowns y condiciones.
// - Outcome: el resultado en el juego (daño, curación, animación).
//
// Este componente maneja la lista de habilidades del personaje,
// verifica si pueden ejecutarse y las activa, aplicando sus efectos.

#include "Public/Components/AbilityComponent.h"

#include "NecroLifeCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAbilityComponent::AbilityAply()
{
	// 1. Verificamos que haya una habilidad y que NO esté en cooldown
	if (CurrentAbility && !isCoolDownAply(CurrentAbility))
	{
		// ... (Aquí va tu lógica para aplicar daño, instanciar proyectiles, animaciones, etc.) ...

		// 2. Calculamos cuándo terminará el cooldown y lo registramos
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float CooldownTime = CurrentAbility->AbilityColdDown; // Asegúrate de tener esta variable en AbilityData
        
		AbilityCooldownEndTimes.Add(CurrentAbility, CurrentTime + CooldownTime);

		// 3. Disparamos tu delegado para actualizar la UI (ej: iniciar la animación de reloj en la barra de habilidades)
		//AbilitySlot.Broadcast(CurrentAbilitySlot);
	}
	AbilitySlot.Broadcast(CurrentAbilitySlot);
}

bool UAbilityComponent::isCoolDownAply(UAbilityData* Ability)
{
	// Validación de seguridad básica
	if (!Ability || !GetWorld())
	{
		return false; 
	}

	// Buscamos si la habilidad tiene un registro de cooldown en el mapa
	if (const float* CooldownEndTime = AbilityCooldownEndTimes.Find(Ability))
	{
		// Si el tiempo actual del juego es menor al tiempo en que termina el cooldown, sigue bloqueada
		if (GetWorld()->GetTimeSeconds() < *CooldownEndTime)
		{
			//para no hacer otro delegado, cambiar algo visual.
			AbilitySlot.Broadcast(CurrentAbilitySlot);
			return true; // ¡La habilidad AÚN ESTÁ en cooldown!
			}
	}

	// Si no está en el mapa o el tiempo ya pasó, no está en cooldown
	return false;
}

TArray<UAbilityData*> UAbilityComponent::getAbilitySlots()
{
	return Abilities;
}

void UAbilityComponent::SelectAbility(int32 Index)
{
	if (!Abilities.IsValidIndex(Index)) return;

	CurrentAbility = Abilities[Index];
    CurrentAbilitySlot=Index;
	// Si ya hay un indicador activo, eliminarlo
	ClearIndicator();
}

void UAbilityComponent::InitPreview()
{
	if (!CurrentAbility) return;

	ANecroLifeCharacter* Owner = Cast<ANecroLifeCharacter>( GetOwner());
	if (!Owner) return;

	// Crear StaticMeshComponent
	MeshComp = NewObject<UStaticMeshComponent>(Owner);
	if (!MeshComp) return;

	MeshComp->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	MeshComp->RegisterComponent();

	// Asignar mesh y material
	if (CurrentAbility->PreviewMesh)
		MeshComp->SetStaticMesh(CurrentAbility->PreviewMesh);
	if (CurrentAbility->PreviewMaterial)
		MeshComp->SetMaterial(0, CurrentAbility->PreviewMaterial);

	// ⚙️ Ajustes visuales y de colisión para preview
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetCastShadow(false);
	MeshComp->bCastDynamicShadow = false;
	MeshComp->bCastStaticShadow = false;
	MeshComp->SetReceivesDecals(false);
	MeshComp->SetRenderCustomDepth(false); // útil si no querés outline

	// Escala y posición inicial
	if (CurrentAbility->bFromCharacter)
	{
		MeshComp->SetWorldScale3D(CurrentAbility->PreviewScale);
		MeshComp->SetWorldLocation(Owner->GetActorLocation());
		MeshComp->SetWorldRotation(Owner->GetActorRotation());
	}else
	{
		MeshComp->SetWorldScale3D(CurrentAbility->PreviewScale);
		MeshComp->SetWorldLocation(Owner->GetAbilityPointer());
		MeshComp->SetWorldRotation(Owner->GetActorRotation());
	}
}

void UAbilityComponent::UpdatePreview(FVector posicionPointer)
{
	if (!CurrentAbility->bFromCharacter)
	{
		MeshComp->SetWorldScale3D(CurrentAbility->PreviewScale);
		MeshComp->SetWorldLocation(posicionPointer);
		MeshComp->AddLocalRotation(FRotator(0.0f, 1.0f, 0.0f));
	}
}

void UAbilityComponent::UpdateIndicator(const FVector& TargetLocation)
{
	if (ActiveIndicator)
	{
		ActiveIndicator->SetWorldLocation(TargetLocation);
	}
}

void UAbilityComponent::ClearIndicator()
{
	if (ActiveIndicator)
	{
		ActiveIndicator->DestroyComponent();
		ActiveIndicator = nullptr;
	}
	if (MeshComp)
	{
		MeshComp->DestroyComponent();
	}
	
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


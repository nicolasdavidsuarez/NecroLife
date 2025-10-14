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
void UAbilityComponent::SelectAbility(int32 Index)
{
	if (!Abilities.IsValidIndex(Index)) return;

	CurrentAbility = Abilities[Index];

	// Si ya hay un indicador activo, eliminarlo
	ClearIndicator();

/*	if (CurrentAbility && CurrentAbility->IndicatorNiagaraSystem)
	{
		// Spawn temporal (sin posición todavía)
   	ActiveIndicator = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			CurrentAbility->IndicatorNiagaraSystem,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			FVector(1.0f)
		);

		// Pasar el radio como variable
		ActiveIndicator->SetVariableFloat(TEXT("Radius"), CurrentAbility->Radius);
	}
*/
	
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


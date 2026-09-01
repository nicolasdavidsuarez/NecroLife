
// AbilityData.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "AbilityData.generated.h"

UCLASS(BlueprintType)
class NECROLIFE_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:

	// Nombre o del Personaje
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FName CharacterName;
	
	// Nombre o ID de la habilidad
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	float AbilityColdDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	float AbilityEnergyNeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	UTexture2D* Icon;

	// Rango máximo del cursor (opcional)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="General")
	float Range = 1000.f;
     /////////////////Habilidad que no sale del character///////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	bool bFromCharacter= true;
	
	// Multiplicador de daño (Ej: 1.0 es normal, 2.0 es el doble)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Stats")
	float DamageMultiplier = 1.0f;

	// Fuerza del empuje (0 = no empuja, 1500 = empuje decente)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Stats")
	float KnockbackForce = 0.0f;
	
	// Radio del área de efecto
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	float Radius = 300.f;

	//////////////////con niagara
	// Sistema visual de Niagara para previsualizar
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	UNiagaraSystem* IndicatorNiagaraSystem;

	/////////////////con static Mesh
	///
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMesh* PreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UMaterialInterface* PreviewMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	FVector PreviewScale = FVector(1.f);
	
	// EL CASILLERO PARA LA ANIMACIÓN DE QUATERNIUS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UAnimMontage* AbilityMontage;
	
};
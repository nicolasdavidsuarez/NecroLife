// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Public/Items/ItemData.h"
#include "InventoryComponent.generated.h"


class AItemGema;


UENUM(BlueprintType)
enum class ETipoEstadisticaGema : uint8
{
	// --- VALOR DIRECTO: ValorMejora = puntos que suma ---
	VidaMaxima          UMETA(DisplayName = "[PUNTOS] Vida Máxima"),
	AtaqueFisico        UMETA(DisplayName = "[PUNTOS] Ataque Físico"),
	Defensa             UMETA(DisplayName = "[PUNTOS] Defensa"),
	EnergiaMaxima       UMETA(DisplayName = "[PUNTOS] Energía Máxima"),

	// --- PORCENTUAL: ValorMejora = número entero (ej: 10 = 10%) ---
	AtaqueFisicoPorcentaje    UMETA(DisplayName = "[%] Ataque Físico"),
	DefensaPorcentaje         UMETA(DisplayName = "[%] Defensa"),
	VidaMaximaPorcentaje      UMETA(DisplayName = "[%] Vida Máxima"),
	Velocidad                 UMETA(DisplayName = "[%] Velocidad de Movimiento"),
	RegeneracionVida          UMETA(DisplayName = "[%] Regeneración de Vida"),
	RegeneracionEnergia       UMETA(DisplayName = "[%] Regeneración de Energía"),

	// --- OTROS ---
	VelocidadAtaquePorcentaje UMETA(DisplayName = "[%] Velocidad de Ataque"),
	DanioDot                  UMETA(DisplayName = "[PUNTOS] Daño en el Tiempo")
};

USTRUCT(BlueprintType)
struct FDatosGema : public FTableRowBase
{
	GENERATED_BODY()

	// El identificador único para buscarla en la base de datos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	FName NombreGema;

	// El nombre que el jugador lee en la interfaz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	FText DescripcionGema;

	// La imagen que se va a mostrar en el inventario y en el slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	UTexture2D* Icon;

	// Qué atributo mejora esta gema (usando el Enum de arriba)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	ETipoEstadisticaGema AtributoAMejorar;

	// Cuánto suma (ej: +10 de Daño, o +50 de Vida)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	float ValorMejora;

	//id de la gema que se puede evolucionar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	FName ID_Row_NextGema;

	//cantida necesaria para mejora
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gemas")
	int32 NeedNextGema;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Cantidad = 1;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPosionChange, int, CantPosiones);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowItem, const TArray<UItemData*>&, ItemsToShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGemsItems, const TArray<FDatosGema>&, GemsToShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGemsItemsInSlots, const TArray<FDatosGema>&, GemsToShowInSlots);





UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	int HealthPosion=0;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void TakeHealthPosion(int Amount);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool UseHealtPosion();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnPosionChange OnPotionChange;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnShowItem OnShowItem;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FGemsItems GemsToShow;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FGemsItemsInSlots GemsToShowInSlots;

	// Esta es la función que llamarás desde el Widget
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RequestAddGemToSlot(FDatosGema gema);

	// Este es el RPC que se ejecutará en el servidor
	UFUNCTION(Server, Reliable)
	void Server_AddGemToSlot(FDatosGema gema);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void PickUp(UItemData* Aitem);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UItemData*> InventoryItems;

	UPROPERTY(ReplicatedUsing= OnRep_GemsItems, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FDatosGema> GemsItems;

	UPROPERTY(ReplicatedUsing= OnRep_GemsItems, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FDatosGema> GemsItemsInInventory;

	UPROPERTY(ReplicatedUsing= OnRep_GemsInSlots, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FDatosGema> GemsInSlots;

	UFUNCTION()
	void OnRep_GemsItems();

	UFUNCTION()
	void OnRep_GemsInSlots();

	UFUNCTION(Server, Reliable)
	void Server_addGemas(FDatosGema gema);
	
	UFUNCTION(Server, Reliable)
	void Server_addGemasInSlot(FDatosGema gema);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
		

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddGems(FDatosGema gema);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FDatosGema> GetGemsItemsInInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<FDatosGema> GetGemsItemsInSlots();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddGemToSlot(FDatosGema gema);

	UFUNCTION(Server, Reliable)
	void Server_QuitGemToSlot(const FDatosGema& Gema);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	
	
	void QuitGemToSlot(FDatosGema gema);

	void GemsNotDuplicates(const TArray<FDatosGema>& SourceArray, TArray<FDatosGema>& TargetArray);

	/**
	 * Intenta craftear/upgradear una gema en la forja.
	 * Busca en el inventario si hay suficientes copias de GemaBase (según NeedNextGema),
	 * las consume y agrega la GemaUpgradeada resultante.
	 *
	 * @param GemaBase      La gema que se quiere upgradear (debe tener ID_NextGema y NeedNextGema válidos).
	 * @param GemaUpgrade   La nueva gema resultante del crafteo (ya cargada desde la DataTable).
	 * @return true si el crafteo fue exitoso, false si no hay materiales suficientes o la gema no es upgradeable.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Forja")
	bool CraftGem(FDatosGema GemaBase, FDatosGema GemaUpgrade);

	// RPC servidor para CraftGem
	UFUNCTION(Server, Reliable)
	void Server_CraftGem(FDatosGema GemaBase, FDatosGema GemaUpgrade);
	
};
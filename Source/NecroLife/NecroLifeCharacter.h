// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Public/Components/UHealthComponent.h"
#include "Public/Components/AttributeComponent.h"
#include "Public/Components/InventoryComponent.h"
#include "Public/Components/AbilityComponent.h"
#include "Engine/EngineTypes.h"
#include "Interface/NecroLifeInterface.h"
#include "Types/NecroLifeTypes.h"
#include "NecroLifeCharacter.generated.h"

struct FActiveQuestData;
struct FGameplayTag;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UUHealthComponent;
class UAttributeComponent;
class UInventoryComponent;
class UAbilityComponent;
class UQuestComponent;
class ANecroLifePlayerState;
class INecroLifeInterface;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMostrarInventario);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMostrarNuevaGema, FDatosGema, DatosGema);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowForgeInventory, const TArray<FDatosGema>&, GemasDisponibles);

UCLASS(abstract)
class ANecroLifeCharacter : public ACharacter, public INecroLifeInterface
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UCameraComponent* FollowCamera;

public:
    ANecroLifeCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
    TObjectPtr<UBoxComponent> BoxCollision;
    
   // UPROPERTY(BlueprintAssignable, Category="Muerte")
    //FOnDie OnDied;
    
    UPROPERTY(BlueprintAssignable, Category="Mostrar Inventario|UI")
    FMostrarInventario ShowInventory;
    
    UPROPERTY(BlueprintAssignable, Category="Mostrar Inventario|UI")
    FOnMostrarNuevaGema ShowNuevaGema;

    // Delegate para abrir el inventario de la forja (networking)
    UPROPERTY(BlueprintAssignable, Category="Mostrar Inventario|UI")
    FOnShowForgeInventory OnShowForgeInventory;

    // --- Componentes ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UUHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UAttributeComponent* Attribute;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UInventoryComponent* Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UAbilityComponent* Ability;

    // QuestComponent movido al GameState para coop
    // UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    // UQuestComponent* QuestComponent;

    UPROPERTY(BlueprintReadOnly, Category="Components")
    TObjectPtr<UQuestComponent> CachedQuestComponent;

    UPROPERTY(BlueprintReadOnly, Category="Player State")
    TObjectPtr<ANecroLifePlayerState> MyPlayerState;

    UPROPERTY(BlueprintReadWrite, Category="UI")
    UUserWidget* HubWidget;

    // --- RPCs de red (networking) ---
    UFUNCTION(Server, Reliable)
    void Server_ActualizarProgresoMision(FGameplayTag ObjectiveID, int32 Amount);
    void Server_ActualizarProgresoMision_Implementation(FGameplayTag ObjectiveID, int32 Amount);
    
    UFUNCTION(Client, Reliable)
    void Client_MostrarNuevaGema(FDatosGema Data);
    
    UFUNCTION(Server, Reliable)
    void Server_CancelQuest(ANecroLifeNpcBasic* Npc);

    UFUNCTION(Server, Reliable)
    void Server_NextQuest(ANecroLifeNpcBasic* Npc);
    
    UFUNCTION(Client, Reliable)
    void Client_SyncQuestUI(const TArray<FQuestUIData>& QuestList);
    
    UFUNCTION()
    void MostarNuevaGema(FDatosGema gemaData);

    UFUNCTION(Server, Reliable)
    void Server_AgregarMision(UQuestData* QuestData);
    void Server_AgregarMision_Implementation(UQuestData* QuestData);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_AplyAction();

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_AplyAbility();

    UFUNCTION(Server, Reliable)
    void Server_TakePosion();

    // --- Dash (networking) ---
    UFUNCTION(Server, Reliable)
    void Server_Dash(FVector DashDir);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_PlayCombatMontage(UAnimMontage* MontageToPlay);

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_DashFX();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StopDashFX();

    // --- Server RPC para combate ---
    UFUNCTION(Server, Reliable)
    void Server_PlayCombatMontage(UAnimMontage* MontageToPlay);

    void PerformDashLogic(FVector DashDir);
    void Local_DashFX();

    // --- Blueprint Events ---
    UFUNCTION(BlueprintImplementableEvent)
    void SetCoolDownAbility(int32 slot);

    UFUNCTION(BlueprintImplementableEvent)
    void SetAbilitySlot(int32 CoolDownAbility);
    
    UFUNCTION(BlueprintImplementableEvent)
    void Bp_SpawnReward();
    
    //Logica de muerte
    UFUNCTION()
    void Die();
    
    UPROPERTY(ReplicatedUsing = OnRep_IsDead)
    bool bIsDead = false;    
    
    UFUNCTION()
    void OnRep_IsDead();
    
    UFUNCTION(BlueprintImplementableEvent)
    void BP_OnDie();

protected:
    // --- Inputs ---
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* RunAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* JumpAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* CameraBoomAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* DashAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* InteractAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseLookAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* AbilityAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* AbilityCancelAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* Action;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* ApplyPosion;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* OpenInventory;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* LookAction;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseRightDown;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseRightUp;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseMiddleDown;
    UPROPERTY(EditAnywhere, Category="Input")
    UInputAction* MouseMiddleUp;

    // --- Habilidades ---

    UPROPERTY(EditAnywhere, Category="Combat|Abilities")
    TSubclassOf<AActor> PalaProjectileClass;

    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void EjecutarLanzamientoPala();
    
    // Timer para el ataque cargado
    FTimerHandle ChargeAttackTimer;

    // Función que saca a la animación del loop y ejecuta el golpe
    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void LiberarGolpePoderoso();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
    float AbilityPointerMaxDistance = 300.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Ability")
    FVector CachedAbilityPointer;

    // --- Cámara (de los compañeros) ---
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float MaxPitch = -45;
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float MinPitch = -25;
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float MaxArmLenght = 1200;
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float MinArmLenght = 200;

    UPROPERTY(EditAnywhere, Category="Camera in game")
    float AutoAlignDelay = 1.5f;
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float AutoAlignSpeed = 2.0f;
    float TimeSinceLastCameraInput = 0.0f;
    void HandleCameraAutoAlignment(float DeltaTime);

    // --- Targeting/Lock-on (de los compañeros) ---
    UPROPERTY(BlueprintReadOnly, Category="Combat|Targeting")
    AActor* CurrentTarget;
    UPROPERTY(BlueprintReadWrite, Category="Combat|Targeting")
    bool bIsTargeting = false;
    UPROPERTY(EditAnywhere, Category="Combat|Targeting")
    float TargetingRadius = 1500.0f;

    // --- Centrado de cámara (de los compañeros) ---
    bool bIsCenteringCamera = false;
    FRotator TargetCenterRotation;
    UPROPERTY(EditAnywhere, Category="Camera in game")
    float CameraCenterSpeed = 25.0f;

    UFUNCTION(BlueprintCallable, Category="Combat|Targeting")
    void ToggleTargeting();
    UFUNCTION(BlueprintCallable, Category="Combat|Targeting")
    void SwitchTargetLeft();
    UFUNCTION(BlueprintCallable, Category="Combat|Targeting")
    void SwitchTargetRight();

    // --- Combate y animaciones (de los compañeros) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Equipment")
    class UMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|VFX")
    class UNiagaraSystem* HitVFX;

    uint16 DashRootMotionID;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combate|Animaciones")
    UAnimMontage* DashMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combate|Animaciones")
    UAnimMontage* DashAttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combate|Animaciones")
    UAnimMontage* AerialAttackMontage;

    
    // --- Sistema de combos (de los compañeros) ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|State")
    bool bIsAttacking;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|State")
    int32 AttackCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|State")
    bool bCanRotateDuringAttack = false;

    UFUNCTION(BlueprintCallable, Category="Combat|Movement")
    void EnableAttackRotation();

    UFUNCTION(BlueprintCallable, Category="Combat|Movement")
    void DisableAttackRotation();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Animations")
    TArray<UAnimMontage*> ComboMontages;

    FTimerHandle ComboResetTimer;
    void ResetCombo();

    // --- Materiales para invisibilidad del dash (de los compañeros) ---
    UPROPERTY()
    TArray<class UMaterialInterface*> OriginalMaterials;
    UPROPERTY()
    TArray<class UMaterialInterface*> OriginalWeaponMaterials;

    // --- Cached components ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UAttributeComponent* CachedAttributeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
    UInventoryComponent* CachedInventoryComponent;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetBoomLength(const FInputActionValue& Value);
    void AbilityEnabled(const FInputActionValue& InputActionValue);
    void AbilityDisambled(const FInputActionValue& InputActionValue);
    void AplyAction();
    void OnRightMouseDown();
    void OnRightMouseUp();
    void OnMiddleMouseUp();
    void OnMiddleMouseDown();
    void RunActivated(const FInputActionValue& Value);
    void TakePosion();
    void Interact();
    void InventoryInput();
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
public:
    UFUNCTION(BlueprintCallable)
    void ShowForgeInventoryAction();


    UFUNCTION(BlueprintCallable)
    FVector GetAbilityPointer() const { return CachedAbilityPointer; }
    FVector GetAbilityPointerPosition() const { return CachedAbilityPointer; }

    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoMove(float Right, float Forward);
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoLook(float Yaw, float Pitch);
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void LookAt(FVector TargetLocation);
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoJumpStart();
    UFUNCTION(BlueprintCallable, Category="Input")
    virtual void DoJumpEnd();

    UPROPERTY()
    AActor* CurrentInteractable;

    virtual void PossessedBy(AController* NewController) override;

    UFUNCTION(BlueprintCallable, Category="Interact")
    void SetCurrentInteractable(AActor* NewInteractable) { CurrentInteractable = NewInteractable; }
    UFUNCTION(BlueprintCallable, Category="Interact")
    void ClearCurrentInteractable() { CurrentInteractable = nullptr; }
    UFUNCTION(BlueprintCallable, Category="Interact")
    AActor* getCurrentInteractable() { return CurrentInteractable; }
    UFUNCTION(BlueprintCallable, Category="Interact")
    void AddCurrentQuest();
    UFUNCTION(BlueprintCallable, Category="Interact")
    void CancelCurrentQuest();
    UFUNCTION(BlueprintCallable, Category="Interact")
    bool ShowDialogue(FDialogLine CurrentLine);

    // --- Funciones de ataque ---
    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void ExecuteAttackHit();

    // Elevación extra al empujar enemigos (0.0 = empuje plano, 1.0 = 45 grados hacia arriba)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    float KnockbackVerticalOffset = 0.75f;

    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void ExecuteAbilityHit();
    
    // Función para atrapar enemigos en área (Habilidad 3)
    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void ExecuteAreaRoot();
    
    // Radio de efecto para la habilidad de inmovilización (Habilidad 3)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    float AbilityRootRadius = 400.0f;
    
    // --- Habilidad 4 (Ametralladora de Palas) ---
    
    // Nueva ranura exclusiva para la pala lanza (así no pisa a la shuriken)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    TSubclassOf<AActor> PalaAmetralladoraClass;

    UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
    void StartSpadeMachineGun();

    // Cantidad de palas a disparar durante la animación
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    int32 PalasToSpawn = 10;

    // Velocidad de disparo (0.1 = 10 palas por segundo)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    float FireRate = 0.1f;

    // Dispersión para que no salgan en línea recta perfecta
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    float SpreadAngle = 5.0f;

    // Variables internas del cronómetro
    int32 PalasDisparadas = 0;
    FTimerHandle MachineGunTimer;
    
    void FireSingleSpade();

    UFUNCTION(BlueprintCallable, Category="Combat|State")
    void ResetAttackState();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Abilities")
    FName AmetralladoraSocketName = FName("Socket_Torso");
    
    
    
    // --- Ataque en Picada (Plunging Attack) ---

    UPROPERTY(BlueprintReadWrite, Category = "Combate|Picada")
    bool bIsPlunging = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Picada")
    float PlungeForwardForce = 1500.f; // Fuerza hacia adelante

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Picada")
    float PlungeDownwardForce = 2500.f; // Fuerza hacia abajo (gravedad extra)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Picada")
    float PlungeDamageRadius = 300.f; // Tamaño de la explosión al caer

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate|Picada")
    float PlungeDamageMultiplier = 1.1f; // 1.0 es daño normal, 1.5 es 50% más fuerte, etc.

    // Funciones del ataque
    void StartPlungingAttack();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void ExecutePlungeHit();
    
    // Sobreescribimos la función nativa de Unreal para saber cuándo tocamos el piso
    virtual void Landed(const FHitResult& Hit) override;
    
   
    // Memoria para no hacerle daño al mismo enemigo 2 veces en un solo swing
    UPROPERTY()
    TArray<AActor*> DamagedActors;
    
    // Limpia la memoria al iniciar el golpe
    UFUNCTION(BlueprintCallable, Category="Combat")
    void ClearWeaponHitMemory();

    // Procesa el impacto y el daño
    UFUNCTION(BlueprintCallable, Category="Combat")
    void ApplyWeaponHit(AActor* HitActor);
    
    
    // --- VFX / Audio del dash (de los compañeros) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|VFX")
    class UNiagaraSystem* DashVFX;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|Audio")
    class USoundBase* DashSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat|VFX")
    class UMaterialInterface* TransparentMaterial;

    FVector Direction;
    FRotator CurrentRotation, TargetRotation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|State")
    bool bIsInvincible;

    UPROPERTY(BlueprintReadWrite, Category="Combate|Estados")
    bool bIsDashing = false;

    bool bShowInventory = false;
    bool bShowForgeInventory = false;  // networking
    bool bCanDash = true;
    bool bMouseRightDown = false;
    bool bMouseMiddleDown = false;
    bool bIsRunning = false;

    FTimerHandle DashTimerHandle;
    FTimerHandle CooldownTimerHandle;

    bool bEnabledAbility = false;
    bool bLookAt = false;

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Override de TakeDamage de Unreal (de los compañeros, para invencibilidad)
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser) override;

    void ShowMsg(FString Msg);
    void UpdateAbilityPointer();
    void LookToCastAbility();
    void Dash();
    void StopDash();

    UFUNCTION(BlueprintCallable)
    void SetUIState(bool bIsTalking);
    
    bool bCanMove=true;

    // Callback cuando el AttributeComponent recalcula stats (de los compañeros)
    UFUNCTION()
    void OnAtributosActualizados(const FEstadisticasPersonaje& NuevosAtributos);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    class UInputMappingContext* InputMapping;
    
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    

    
    
};
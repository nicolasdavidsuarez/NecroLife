// Copyright Epic Games, Inc. All Rights Reserved.

#include "NecroLifeCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/Components/RPGHelper.h"
#include "NecroLife.h"
#include "Public/Components/AttributeComponent.h"
#include "CollisionQueryParams.h"
#include "NecroLifeGameState.h"
#include "NecroLifePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/QuestComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/PlayerState.h"
#include "Interface/NecroLifeInterface.h"
#include "NPC/NecroLifeEnemyBasic.h"
#include "NPC/NecroLifeNpcBasic.h"
#include "Widgets/NecroLifeHud.h"



// ============================================================
// Constructor
// ============================================================

ANecroLifeCharacter::ANecroLifeCharacter()
{
    HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));
    Inventory       = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    Ability         = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
    Attribute       = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributesComponent"));
    // QuestComponent movido al GameState para coop — no se crea acá

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;

    GetCharacterMovement()->bOrientRotationToMovement    = true;
    GetCharacterMovement()->RotationRate                 = FRotator(0.0f, 400.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity                = 500.f;
    GetCharacterMovement()->AirControl                   = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed                 = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed           = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking   = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling   = 1500.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength         = 800.0f;
    CameraBoom->bDoCollisionTest        = true;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bInheritPitch           = true;
    CameraBoom->bInheritYaw             = true;
    // Lag de cámara (de los compañeros — da sensación más pulida)
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->CameraRotationLagSpeed   = 5.0f;
    CameraBoom->bEnableCameraLag         = true;
    CameraBoom->CameraLagSpeed           = 5.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCol"));
    BoxCollision->SetBoxExtent(FVector(100, 100, 100), true);

        
    // Networking
    bReplicates = true;
    SetReplicatingMovement(true);
    GetMesh()->SetIsReplicated(true);
}

// ============================================================
// BeginPlay
// ============================================================

void ANecroLifeCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
            {
                if (InputMapping)
                    Subsystem->AddMappingContext(InputMapping, 0);
            }
        }
    }

    // Las gemas van por el InventoryComponent del PlayerState → CachedAttributeComponent es la
    // fuente autoritativa de stats. No bindeamos Attribute del personaje para evitar que
    // sobreescriba MaxWalkSpeed con Velocidad base (sin gemas) después de que CachedAttribute lo
    // seteó correctamente con el bonus de gemas.
    // El recálculo inicial en servidor usa el Inventory del personaje (vacío al inicio; el del
    // PlayerState se encarga de los stats reales en su propio BeginPlay).
    if (HasAuthority() && Attribute)
    {
        Attribute->RecalcularEstadisticas(Inventory->GemsInSlots);
    }
}

// ============================================================
// PossessedBy
// ============================================================

void ANecroLifeCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    MyPlayerState = GetPlayerState<ANecroLifePlayerState>();
    if (MyPlayerState)
    {
        CachedAttributeComponent = MyPlayerState->GetAttributeComponent();
        CachedInventoryComponent = MyPlayerState->GetInventoryComponent();
        // CachedQuestComponent movido al GameState para coop
        // CachedQuestComponent = MyPlayerState->GetQuestComponent();

        if (!CachedAttributeComponent || !CachedInventoryComponent || !CachedQuestComponent)
        {
            UE_LOG(LogTemp, Error, TEXT("Character %s failed to cache all components from PlayerState."), *GetName());
        }

        // Si las gemas se equipan via el InventoryComponent del PlayerState, el delegate
        // que recalcula es el de CachedAttributeComponent, no el de Attribute (char).
        // Bindeamos aqui para que MaxWalkSpeed se actualice en ambos casos.
        if (CachedAttributeComponent)
        {
            CachedAttributeComponent->OnAtributosActualizados.AddDynamic(this, &ANecroLifeCharacter::OnAtributosActualizados);

            // Copiar los valores base del Attribute del personaje (configurados en BP_Huesos)
            // al CachedAttributeComponent del PlayerState, que es el que recalcula con gemas.
            // Esto permite que BaseVelocity, BaseLife, etc. se configuren desde el Blueprint
            // del personaje y se reflejen correctamente en el sistema de stats.
            if (HasAuthority() && Attribute)
            {
                CachedAttributeComponent->BaseVelocity      = Attribute->BaseVelocity;
                CachedAttributeComponent->BaseLife          = Attribute->BaseLife;
                CachedAttributeComponent->BaseAttack        = Attribute->BaseAttack;
                CachedAttributeComponent->DefenseBase       = Attribute->DefenseBase;
                CachedAttributeComponent->BaseEnergy        = Attribute->BaseEnergy;
                CachedAttributeComponent->VelocityAttackBase = Attribute->VelocityAttackBase;
                CachedAttributeComponent->BaseRegenVida     = Attribute->BaseRegenVida;
                CachedAttributeComponent->BaseRegenEnergia  = Attribute->BaseRegenEnergia;

                const TArray<FDatosGema>& Gemas = CachedInventoryComponent
                    ? CachedInventoryComponent->GemsInSlots
                    : TArray<FDatosGema>();
                CachedAttributeComponent->RecalcularEstadisticas(Gemas);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character %s possessed but failed to get PlayerState."), *GetName());
    }
}

// ============================================================
// Tick
// ============================================================

void ANecroLifeCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateAbilityPointer();
    LookToCastAbility();

    TimeSinceLastCameraInput += DeltaTime;

    // --- Lógica de cámara (de los compañeros) ---
    if (bIsTargeting && CurrentTarget)
    {
        // MODO TARGETING: apuntamos la cámara al enemigo fijado
        ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(CurrentTarget);
        if (!Enemy || !Enemy->IsAlive())
        {
            ToggleTargeting(); // El enemigo murió, soltamos el target
        }
        else
        {
            FVector StartLoc = FollowCamera->GetComponentLocation();
            FVector TargetLoc = CurrentTarget->GetActorLocation();
            TargetLoc.Z -= 50.0f; // Apuntamos al pecho, no a los pies
            FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);
            FRotator NewRot = FMath::RInterpTo(GetControlRotation(), TargetRot, DeltaTime, 10.0f);
            GetController()->SetControlRotation(NewRot);
        }
    }
    else if (bIsCenteringCamera)
    {
        // MODO CENTRADO: cámara se acomoda detrás del personaje
        FRotator CurrentRot = GetControlRotation();
        FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetCenterRotation, DeltaTime, CameraCenterSpeed);
        GetController()->SetControlRotation(NewRot);
        if (FMath::IsNearlyEqual(NewRot.Yaw, TargetCenterRotation.Yaw, 2.0f))
            bIsCenteringCamera = false;
    }
    else
    {
        // MODO LIBRE: auto-alineación suave
        HandleCameraAutoAlignment(DeltaTime);
    }
}

// ============================================================
// Cámara y movimiento
// ============================================================

void ANecroLifeCharacter::SetBoomLength(const FInputActionValue& Value)
{
    FVector2D InputVector = Value.Get<FVector2D>();
    float armLength = CameraBoom->TargetArmLength;

    if (InputVector.X > 0 && armLength < MaxArmLenght)
    {
        armLength += CameraBoom->TargetArmLength * 0.05f;
        CameraBoom->TargetArmLength = armLength;
    }
    else
    {
        armLength -= CameraBoom->TargetArmLength * 0.05f;
        if (armLength > MinArmLenght)
            CameraBoom->TargetArmLength = armLength;
    }
}

void ANecroLifeCharacter::Move(const FInputActionValue& Value)
{
    if (bShowInventory) return;
    if (bShowForgeInventory) return;
    if (!bCanMove)return;
    FVector2D MovementVector = Value.Get<FVector2D>();
    DoMove(MovementVector.X, MovementVector.Y);
}

void ANecroLifeCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ANecroLifeCharacter::DoLook(float Yaw, float Pitch)
{
    // Solo reiniciamos el timer y rotamos la cámara SI el clic derecho está presionado
    if (GetController() != nullptr && bMouseRightDown)
    {
        // Reiniciamos el timer de auto-alineación y cancelamos centrado
        TimeSinceLastCameraInput = 0.0f;
        bIsCenteringCamera = false;

        AddControllerYawInput(Yaw);
        
        if (CameraBoom->GetRelativeRotation().Pitch + Pitch >= MaxPitch &&
            CameraBoom->GetRelativeRotation().Pitch + Pitch <= MinPitch)
        {
            CameraBoom->AddRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
        }
        else
        {
            if (Pitch != 0)
            {
                Pitch *= -1.0f;
                CameraBoom->AddRelativeRotation(FRotator(Pitch, 0.0f, 0.0f));
            }
        }
    }
}

void ANecroLifeCharacter::LookAt(FVector TargetLocation)
{
    FVector StartLocation = GetPawnViewLocation();
    FVector LookDirection = TargetLocation - StartLocation;
    FRotator LookAtRot = LookDirection.Rotation();
    DoLook(LookAtRot.Yaw, LookAtRot.Pitch);
    if (AController* CharController = GetController())
        CharController->SetControlRotation(LookAtRot);
}

void ANecroLifeCharacter::DoMove(float Right, float Forward)
{
    if (GetController() != nullptr)
    {
        const FRotator Rotation = GetController()->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // 1. Caminata Normal (Bloqueada si ataca, cae o castea)
        if (!bIsAttacking && !bIsPlunging && !bEnabledAbility)
        {
            AddMovementInput(ForwardDirection, Forward);
            AddMovementInput(RightDirection, Right);
        }
        // 2. Lógica de Redirección (Pivote en el lugar)
        else if (bIsAttacking && bCanRotateDuringAttack && (Right != 0.0f || Forward != 0.0f))
        {
            // Calculamos hacia dónde apunta el joystick
            FVector DesiredDirection = (ForwardDirection * Forward) + (RightDirection * Right);
            DesiredDirection.Normalize();

            // Interpolamos la rotación para que no sea robótica/instantánea
            FRotator TargetRot = DesiredDirection.Rotation();
            FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 15.0f); 
            SetActorRotation(NewRot);
        }
    }
}

void ANecroLifeCharacter::DoJumpStart() 
{ 
    // Si Huesos está ocupado en una animación de combate, no puede saltar
    if (bIsAttacking || bIsDashing || bIsPlunging || bEnabledAbility || !bCanMove) return;

    Jump(); 
}
void ANecroLifeCharacter::DoJumpEnd()   { StopJumping(); }

void ANecroLifeCharacter::OnRightMouseDown()  { bMouseRightDown = true; }
void ANecroLifeCharacter::OnRightMouseUp()    { bMouseRightDown = false; }
void ANecroLifeCharacter::OnMiddleMouseDown() { bMouseMiddleDown = true; }
void ANecroLifeCharacter::OnMiddleMouseUp()   { bMouseMiddleDown = false; }

void ANecroLifeCharacter::RunActivated(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        bIsRunning = !bIsRunning;
        // Usar CachedAttributeComponent (PlayerState) que tiene el bonus de gemas aplicado.
        // Fallback al Attribute del personaje si todavía no está disponible.
        const float V = (CachedAttributeComponent && CachedAttributeComponent->StatsSincronizadas.Velocidad > 0.f)
            ? CachedAttributeComponent->StatsSincronizadas.Velocidad
            : Attribute->StatsSincronizadas.Velocidad;
        GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? V * 200.0f : V * 50.0f;
    }
}

void ANecroLifeCharacter::HandleCameraAutoAlignment(float DeltaTime)
{
    // Bloqueamos la auto-alineación de la cámara libre durante los ataques y habilidades
    if (bEnabledAbility || bIsAttacking || bIsPlunging || AttackCount > 0) return;

    FVector Velocity = GetVelocity();
    if (Velocity.SizeSquared2D() < 10.0f) return;

    // Solo alineamos si el personaje mira hacia adelante o en diagonal hacia adelante
    FVector CameraForward = FollowCamera->GetForwardVector();
    CameraForward.Z = 0.0f;
    CameraForward.Normalize();
    FVector VelocityDir = Velocity.GetSafeNormal();

    if (FVector::DotProduct(CameraForward, VelocityDir) < 0.5f) return;

    if (TimeSinceLastCameraInput >= AutoAlignDelay)
    {
        FRotator CurrentControlRot = GetControlRotation();
        FRotator TargetRot = Velocity.Rotation();
        FRotator NewControlRot = CurrentControlRot;
        NewControlRot.Yaw = FMath::RInterpTo(CurrentControlRot, TargetRot, DeltaTime, AutoAlignSpeed).Yaw;
        if (AController* CharController = GetController())
            CharController->SetControlRotation(NewControlRot);
    }
}


// ============================================================
// RPCs de networking (tuyas)
// ============================================================

void ANecroLifeCharacter::Server_AplyAbility_Implementation()
{
    TArray<FOverlapResult> Overlaps;
    FVector Origin = GetActorLocation();
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(300.f);

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    if (!bHit) return;

    FVector Forward = GetActorForwardVector();

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

        if (AngleToTarget <= 45.f)
        {
            URPGHelper::ApplyDamage(Other, 100);
            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, EnemyBasic->EsenciasAlMorir);
                Server_ActualizarProgresoMision(EnemyBasic->GetTag(), 1);
            }
        }
    }
}
bool ANecroLifeCharacter::Server_AplyAbility_Validate() { return true; }

void ANecroLifeCharacter::Server_AplyAction_Implementation()
{
    TArray<FOverlapResult> Overlaps;
    FVector Origin = GetActorLocation();
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(100, 100, 100));

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    if (!bHit) return;

    FVector Forward = GetActorForwardVector();

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

        if (AngleToTarget <= 45.f)
        {
            URPGHelper::ApplyDamage(Other, 10);
            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, EnemyBasic->EsenciasAlMorir);
                Server_ActualizarProgresoMision(EnemyBasic->GetTag(), 1);
            }
        }
    }
}
bool ANecroLifeCharacter::Server_AplyAction_Validate() { return true; }

void ANecroLifeCharacter::Server_ActualizarProgresoMision_Implementation(FGameplayTag ObjectiveID, int32 Amount)
{
    // QuestComponent movido al GameState para coop
    ANecroLifeGameState* GS = GetWorld()->GetGameState<ANecroLifeGameState>();
    if (GS && GS->QuestComponent)
    {
        GS->QuestComponent->UpdateQuestProgress(ObjectiveID, Amount);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
            TEXT("El Servidor recibió la interacción y actualizó el GameState"));
    }
}

void ANecroLifeCharacter::Client_MostrarNuevaGema_Implementation(FDatosGema Data)
{
    MostarNuevaGema(Data);
}



void ANecroLifeCharacter::Client_SyncQuestUI_Implementation(const TArray<FQuestUIData>& QuestList)
{
    if (UNecroLifeHud* HUD = Cast<UNecroLifeHud>(HubWidget))
    {
        HUD->ActualizarQuestList(QuestList); // ← tu función existente
    }
}

void ANecroLifeCharacter::MostarNuevaGema(FDatosGema gemaData)
{
    ShowNuevaGema.Broadcast(gemaData);
}

void ANecroLifeCharacter::Server_AgregarMision_Implementation(UQuestData* QuestData)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
           TEXT("Entro a Server_AgregarMision_Implementation"));
    ANecroLifeGameState* GS = GetWorld()->GetGameState<ANecroLifeGameState>();
    if (GS && GS->QuestComponent)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange,
            TEXT("Entro a GS->QuestComponent, para AddQuest"));
        GS->QuestComponent->AddQuest(QuestData);
    }
}

// ============================================================
// Targeting / Lock-on (de los compañeros)
// ============================================================

void ANecroLifeCharacter::ToggleTargeting()
{
    if (bIsTargeting)
    {
        if (CurrentTarget)
        {
            if (ANecroLifeEnemyBasic* OldEnemy = Cast<ANecroLifeEnemyBasic>(CurrentTarget))
                OldEnemy->OnTargetStatusChanged(false);
        }
        bIsTargeting = false;
        CurrentTarget = nullptr;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
        return;
    }

    FVector StartLoc = GetActorLocation();
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TargetingRadius);
    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, StartLoc, FQuat::Identity, ECC_Pawn, Sphere);

    if (bHit)
    {
        ANecroLifeEnemyBasic* BestTarget = nullptr;
        float HighestDot = -1.0f;
        FVector CamForward = FollowCamera->GetForwardVector();
        FVector CamLoc = FollowCamera->GetComponentLocation();

        for (auto& Result : Overlaps)
        {
            AActor* OtherActor = Result.GetActor();
            if (OtherActor == this) continue;
            ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor);
            if (Enemy && Enemy->IsAlive())
            {
                FVector DirToEnemy = (Enemy->GetActorLocation() - CamLoc).GetSafeNormal();
                float Dot = FVector::DotProduct(CamForward, DirToEnemy);
                if (Dot > 0.0f && Dot > HighestDot)
                {
                    HighestDot = Dot;
                    BestTarget = Enemy;
                }
            }
        }

        if (BestTarget != nullptr)
        {
            CurrentTarget = BestTarget;
            bIsTargeting = true;
            BestTarget->OnTargetStatusChanged(true);
            GetCharacterMovement()->bOrientRotationToMovement = false;
            bUseControllerRotationYaw = true;
        }
        else
        {
            bIsCenteringCamera = true;
            TargetCenterRotation = GetActorRotation();
            TargetCenterRotation.Pitch = GetControlRotation().Pitch;
            TargetCenterRotation.Roll  = GetControlRotation().Roll;
        }
    }
}

void ANecroLifeCharacter::SwitchTargetRight()
{
    if (!bIsTargeting || !CurrentTarget) return;

    FVector StartLoc = GetActorLocation();
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TargetingRadius);
    GetWorld()->OverlapMultiByChannel(Overlaps, StartLoc, FQuat::Identity, ECC_Pawn, Sphere);

    ANecroLifeEnemyBasic* BestTarget = nullptr;
    float HighestForwardDot = -1.0f;
    FVector CamRight   = FollowCamera->GetRightVector();
    FVector CamForward = FollowCamera->GetForwardVector();
    FVector CamLoc     = FollowCamera->GetComponentLocation();

    for (auto& Result : Overlaps)
    {
        AActor* OtherActor = Result.GetActor();
        if (OtherActor == this || OtherActor == CurrentTarget) continue;
        ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor);
        if (Enemy && Enemy->IsAlive())
        {
            FVector DirToEnemy = (Enemy->GetActorLocation() - CamLoc).GetSafeNormal();
            float RightDot   = FVector::DotProduct(CamRight, DirToEnemy);
            float ForwardDot = FVector::DotProduct(CamForward, DirToEnemy);
            if (RightDot > 0.1f && ForwardDot > HighestForwardDot)
            {
                HighestForwardDot = ForwardDot;
                BestTarget = Enemy;
            }
        }
    }

    if (BestTarget != nullptr)
    {
        if (ANecroLifeEnemyBasic* OldEnemy = Cast<ANecroLifeEnemyBasic>(CurrentTarget))
            OldEnemy->OnTargetStatusChanged(false);
        CurrentTarget = BestTarget;
        BestTarget->OnTargetStatusChanged(true);
    }
}

void ANecroLifeCharacter::SwitchTargetLeft()
{
    if (!bIsTargeting || !CurrentTarget) return;

    FVector StartLoc = GetActorLocation();
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TargetingRadius);
    GetWorld()->OverlapMultiByChannel(Overlaps, StartLoc, FQuat::Identity, ECC_Pawn, Sphere);

    ANecroLifeEnemyBasic* BestTarget = nullptr;
    float HighestForwardDot = -1.0f;
    FVector CamRight   = FollowCamera->GetRightVector();
    FVector CamForward = FollowCamera->GetForwardVector();
    FVector CamLoc     = FollowCamera->GetComponentLocation();

    for (auto& Result : Overlaps)
    {
        AActor* OtherActor = Result.GetActor();
        if (OtherActor == this || OtherActor == CurrentTarget) continue;
        ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor);
        if (Enemy && Enemy->IsAlive())
        {
            FVector DirToEnemy = (Enemy->GetActorLocation() - CamLoc).GetSafeNormal();
            float RightDot   = FVector::DotProduct(CamRight, DirToEnemy);
            float ForwardDot = FVector::DotProduct(CamForward, DirToEnemy);
            if (RightDot < -0.1f && ForwardDot > HighestForwardDot)
            {
                HighestForwardDot = ForwardDot;
                BestTarget = Enemy;
            }
        }
    }

    if (BestTarget != nullptr)
    {
        if (ANecroLifeEnemyBasic* OldEnemy = Cast<ANecroLifeEnemyBasic>(CurrentTarget))
            OldEnemy->OnTargetStatusChanged(false);
        CurrentTarget = BestTarget;
        BestTarget->OnTargetStatusChanged(true);
    }
}

// ============================================================
// Combate, habilidades y combos (de los compañeros)
// ============================================================

void ANecroLifeCharacter::AbilityEnabled(const FInputActionValue& InputActionValue)
{
    if (bIsAttacking || bIsDashing || bIsPlunging) return;

    // pressedKeys vale 0 si apretás el "1", y vale 1 si apretás el "2"
    int32 pressedKeys = static_cast<int32>(InputActionValue.Get<float>()) - 1;
    Ability->SelectAbility(pressedKeys);

    if (!Ability->isCoolDownAply(Ability->CurrentAbility))
    {
        if (Ability->CurrentAbility && Ability->CurrentAbility->AbilityMontage)
        {
            // Auto-apuntado (lo que ya teníamos)
            if (bIsTargeting && CurrentTarget)
            {
                FVector TargetLoc = CurrentTarget->GetActorLocation();
                TargetLoc.Z = GetActorLocation().Z;
                SetActorRotation((TargetLoc - GetActorLocation()).Rotation());
            }

            // Reproduce la animación (Acá arranca la sección "Inicio" y se traba loopeando "Carga")
            PlayAnimMontage(Ability->CurrentAbility->AbilityMontage);
            Server_PlayCombatMontage(Ability->CurrentAbility->AbilityMontage); 
            
            Ability->AbilityAply();
            bIsAttacking = true; 

            // ==========================================
            // NUEVO: Lógica específica para el Golpe Poderoso
            // ==========================================
            // Si apretó la tecla 2 (índice 1), iniciamos el contador
            if (pressedKeys == 1)
            {
                GetWorldTimerManager().SetTimer(ChargeAttackTimer, this, &ANecroLifeCharacter::LiberarGolpePoderoso, 1.0f, false);
            }
        }
    }
}

void ANecroLifeCharacter::EjecutarLanzamientoPala()
{
    if (PalaProjectileClass)
    {
        // Usamos el mismo socket que ya tenés creado para su mano derecha
        FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("Socket_Mango_Pala")); 
        
        // Hacemos que la pala salga disparada hacia el frente exacto del personaje
        FRotator SpawnRotation = GetActorRotation(); 

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        GetWorld()->SpawnActor<AActor>(PalaProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
}

void ANecroLifeCharacter::LiberarGolpePoderoso()
{
    // Buscamos el motor de animaciones de Huesos
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        // Nos aseguramos de tener una habilidad válida
        if (Ability && Ability->CurrentAbility && Ability->CurrentAbility->AbilityMontage)
        {
            // Magia pura: Le decimos a Unreal que salte instantáneamente a la sección "Golpe"
            AnimInstance->Montage_JumpToSection(FName("Golpe"), Ability->CurrentAbility->AbilityMontage);
        }
    }
}

void ANecroLifeCharacter::AbilityDisambled(const FInputActionValue& InputActionValue)
{
    if (bEnabledAbility)
    {
        GetCharacterMovement()->bOrientRotationToMovement = true;
        bUseControllerRotationYaw = false;
        bEnabledAbility = false;
        Ability->ClearIndicator();
    }
    bLookAt = true;
}

void ANecroLifeCharacter::AplyAction()
{
    if (bShowInventory || CurrentInteractable) return;

    // 1. Ataque en Dash
    if (bIsDashing)
    {
        if (DashAttackMontage)
        {
            StopDash();
            bIsAttacking = true;
            PlayAnimMontage(DashAttackMontage);
            Server_PlayCombatMontage(DashAttackMontage);
            AttackCount = 0;
        }
    }
    // 2. Ataque aéreo en picada (con tu mejora para evitar spam)
    else if (GetCharacterMovement()->IsFalling() || bIsPlunging)
    {
        if (!bIsPlunging && GetVelocity().Z < 0.0f) 
        {
            StartPlungingAttack();
        }
    }
    // 3. Combo de ataques básicos
    else
    {
        if (bIsAttacking || ComboMontages.Num() == 0) return;

        bIsAttacking = true;
        UAnimMontage* MontageToPlay = ComboMontages[AttackCount];
        if (MontageToPlay)
        {
            PlayAnimMontage(MontageToPlay);
            Server_PlayCombatMontage(MontageToPlay);
        }

        AttackCount++;
        if (AttackCount >= ComboMontages.Num())
            AttackCount = 0;

        GetWorldTimerManager().SetTimer(ComboResetTimer, this, &ANecroLifeCharacter::ResetCombo, 1.f, false);
    }
}

void ANecroLifeCharacter::ResetCombo()
{
    AttackCount  = 0;
    bIsAttacking = false;
    bCanRotateDuringAttack = false;
}

void ANecroLifeCharacter::ResetAttackState()
{
    bIsAttacking = false;
    bCanRotateDuringAttack = false;
}

void ANecroLifeCharacter::ExecuteAttackHit()
{
    TArray<FOverlapResult> Overlaps;
    FVector Origin = GetActorLocation();
    FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(100, 100, 100));

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    if (!bHit) return;

    FVector Forward = GetActorForwardVector();
    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

        if (AngleToTarget <= 45.f)
        {
            URPGHelper::ApplyDamage(Other, Attribute->Attack);
            if (HitVFX)
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Other->GetActorLocation());

            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, EnemyBasic->EsenciasAlMorir);
                Server_ActualizarProgresoMision(EnemyBasic->GetTag(), 1);
            }
        }
    }
}

void ANecroLifeCharacter::StartPlungingAttack()
{
    bIsPlunging = true;
    bIsAttacking = true;

    // 1. Modificamos un poco la gravedad para que la caída se sienta más pesada que un salto normal
    GetCharacterMovement()->GravityScale = 2.0f; // Podés jugar con este valor (1.0 es lo normal)

    // 2. Calculamos y aplicamos el impulso diagonal INMEDIATAMENTE
    FVector ForwardLaunch = GetActorForwardVector() * PlungeForwardForce;
    FVector DownwardLaunch = FVector(0.f, 0.f, -PlungeDownwardForce);
    LaunchCharacter(ForwardLaunch + DownwardLaunch, true, true);

    // 3. Reproducimos el Montage del hachazo (que ahora configuraremos en bucle)
    if (AerialAttackMontage)
    {
        PlayAnimMontage(AerialAttackMontage);
        Server_PlayCombatMontage(AerialAttackMontage);
    }
}

void ANecroLifeCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit); 

    if (bIsPlunging)
    {
        bIsPlunging = false;
		
        // Restauramos la gravedad a la normalidad
        GetCharacterMovement()->GravityScale = 1.0f; 

        // Le decimos al Montage que salte a la sección del hachazo final
        if (AerialAttackMontage)
        {
            if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
            {
                AnimInstance->Montage_JumpToSection(FName("Ataque"), AerialAttackMontage);
            }
        }
    }
}

void ANecroLifeCharacter::ExecutePlungeHit()
{
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    TArray<AActor*> OutActors;

    // Calculamos el centro de la explosión (los pies de Huesos)
    // 96.0f es el HalfHeight de tu cápsula
    FVector ImpactPoint = GetActorLocation() - FVector(0.f, 0.f, 96.0f); 

    bool bHit = UKismetSystemLibrary::SphereOverlapActors(
        this,
        ImpactPoint, 
        PlungeDamageRadius,
        ObjectTypes,
        AActor::StaticClass(),
        ActorsToIgnore,
        OutActors
    );

    // Dejamos el debug activo para que calibres el radio visualmente
    UKismetSystemLibrary::DrawDebugSphere(this, ImpactPoint, PlungeDamageRadius, 12, FLinearColor::Red, 1.f, 2.f);

    if (bHit)
    {
        // 1. Verificamos que el componente de atributos exista por seguridad
        if (Attribute) 
        {
            // 2. Calculamos el daño final (Daño Base * Multiplicador)
            float FinalPlungeDamage = Attribute->Attack * PlungeDamageMultiplier;

            for (AActor* HitActor : OutActors)
            {
                if (HitActor && HitActor->IsA<ANecroLifeEnemyBasic>())
                {
                    // 3. Aplicamos el daño calculado dinámicamente
                    URPGHelper::ApplyDamage(HitActor, FinalPlungeDamage);
                }
            }
        }
    }
}

void ANecroLifeCharacter::ExecuteAbilityHit()
{
    float AttackRadius = 300.f;
    float AttackAngle  = 45.f;

    FVector Origin  = GetActorLocation();
    FVector Forward = GetActorForwardVector();
    TArray<FOverlapResult> Overlaps;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    Ability->AbilityAply();
    if (!bHit) return;

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        if (!EnemyBasic || Other == this) continue;

        FVector ToTarget = (EnemyBasic->GetActorLocation() - Origin).GetSafeNormal();
        float Dot = FVector::DotProduct(Forward, ToTarget);
        float AngleToTarget = FMath::RadiansToDegrees(FMath::Acos(Dot));

        if (AngleToTarget <= AttackAngle)
        {
            // 1. Calculamos el daño leyendo el multiplicador de la habilidad actual
            float FinalDamage = Attribute->Attack;
            float Knockback = 0.0f;

            // Verificamos de forma segura que haya una habilidad equipada
            if (Ability && Ability->CurrentAbility)
            {
                FinalDamage *= Ability->CurrentAbility->DamageMultiplier;
                Knockback = Ability->CurrentAbility->KnockbackForce;
            }

            // Aplicamos el daño
            URPGHelper::ApplyDamage(Other, FinalDamage);
            
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Fuerza de empuje: %f"), Knockback));
            
            // 2. Lógica de Empuje (Pushback)
            if (Knockback > 0.0f)
            {
                FVector KnockbackDirection = ToTarget;
                KnockbackDirection.Z = 0.5f; 
                KnockbackDirection.Normalize();

                // PASO CLAVE 1: Clavamos los frenos de la IA y cancelamos su navegación actual
                EnemyBasic->GetCharacterMovement()->StopMovementImmediately();

                // PASO CLAVE 2: Cortamos cualquier animación de ataque para anular el Root Motion
                EnemyBasic->StopAnimMontage();

                // PASO CLAVE 3: Ahora sí, sin resistencia de la IA, lo mandamos a volar
                EnemyBasic->LaunchCharacter(KnockbackDirection * Knockback, true, true);
            }

            // 3. Lógica de muerte (queda igual)
            if (!EnemyBasic->IsAlive())
            {
                URPGHelper::TakeXP(this, EnemyBasic->EsenciasAlMorir);
                Server_ActualizarProgresoMision(EnemyBasic->GetTag(), 1);
            }
        }
    }
}

float ANecroLifeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                       AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsInvincible) return 0.0f;
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// ============================================================
// Puntero de habilidad
// ============================================================

void ANecroLifeCharacter::UpdateAbilityPointer()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    FHitResult HitResult;
    if (PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
    {
        FVector PlayerPos = GetActorLocation();
        FVector ToMouse = HitResult.Location - PlayerPos;
        ToMouse.Z = 0;

        if (ToMouse.Size() > AbilityPointerMaxDistance)
            ToMouse = ToMouse.GetSafeNormal() * AbilityPointerMaxDistance;

        CachedAbilityPointer   = PlayerPos + ToMouse;
        CachedAbilityPointer.Z = PlayerPos.Z;

        if (bEnabledAbility)
            Ability->UpdatePreview(CachedAbilityPointer);
    }
}

void ANecroLifeCharacter::LookToCastAbility()
{
    if (bEnabledAbility)
    {
        FVector PlayerPos = GetActorLocation();
        FVector ToMouse   = CachedAbilityPointer - PlayerPos;
        ToMouse.Z = 0;
        SetActorRotation(ToMouse.Rotation());
    }
}

// ============================================================
// Dash (networking + VFX de los compañeros)
// ============================================================

void ANecroLifeCharacter::Dash()
{
    if (!bCanDash || bIsDashing || bIsAttacking || !Attribute) return;

    ResetCombo();
    StopAnimMontage();

    FVector DashDirection = FVector::ZeroVector;
    FVector LastInput = GetCharacterMovement()->GetLastInputVector();
    DashDirection = LastInput.IsNearlyZero() ? -GetActorForwardVector() : LastInput.GetSafeNormal();

    // Física local al instante (predicción del cliente)
    PerformDashLogic(DashDirection);
    Local_DashFX();

    // Networking
    if (HasAuthority())
        Multicast_DashFX();
    else
        Server_Dash(DashDirection);
}

void ANecroLifeCharacter::Server_Dash_Implementation(FVector DashDir)
{
    PerformDashLogic(DashDir);
    Multicast_DashFX();
}

void ANecroLifeCharacter::PerformDashLogic(FVector DashDir)
{
    bIsDashing   = true;
    bCanDash     = false;
    bIsInvincible = true;

    if (DashRootMotionID != 0)
        GetCharacterMovement()->RemoveRootMotionSourceByID(DashRootMotionID);

    TSharedPtr<FRootMotionSource_ConstantForce> DashForce = MakeShared<FRootMotionSource_ConstantForce>();
    DashForce->InstanceName    = FName("DashForce");
    DashForce->AccumulateMode  = ERootMotionAccumulateMode::Override;
    DashForce->Priority        = 5;
    DashForce->Force           = DashDir * Attribute->DashStrength;
    DashForce->Duration        = Attribute->DashDuration;
    DashForce->FinishVelocityParams.Mode        = ERootMotionFinishVelocityMode::SetVelocity;
    DashForce->FinishVelocityParams.SetVelocity = FVector::ZeroVector;

    DashRootMotionID = GetCharacterMovement()->ApplyRootMotionSource(DashForce);
    GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ANecroLifeCharacter::StopDash, Attribute->DashDuration, false);
}

void ANecroLifeCharacter::Local_DashFX()
{
    if (DashMontage)
        PlayAnimMontage(DashMontage);

    if (DashVFX)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
            DashVFX, GetMesh(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
            EAttachLocation::SnapToTarget, true);
        if (NiagaraComp) NiagaraComp->SetVariableObject(FName("User.SourceMesh"), GetMesh());
    }

    OriginalMaterials.Empty();
    if (TransparentMaterial)
    {
        for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
        {
            OriginalMaterials.Add(GetMesh()->GetMaterial(i));
            GetMesh()->SetMaterial(i, TransparentMaterial);
        }
    }

    if (DashSound)
        UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());

    OriginalWeaponMaterials.Empty();
    if (TransparentMaterial && WeaponMesh)
    {
        for (int32 i = 0; i < WeaponMesh->GetNumMaterials(); ++i)
        {
            OriginalWeaponMaterials.Add(WeaponMesh->GetMaterial(i));
            WeaponMesh->SetMaterial(i, TransparentMaterial);
        }
    }
}

void ANecroLifeCharacter::Multicast_DashFX_Implementation()
{
    // Solo reproducimos en espectadores, el dueño ya lo hizo localmente
    if (!IsLocallyControlled())
        Local_DashFX();
}

void ANecroLifeCharacter::StopDash()
{
    bIsDashing    = false;
    bIsInvincible = false;

    if (DashRootMotionID != 0)
    {
        GetCharacterMovement()->RemoveRootMotionSourceByID(DashRootMotionID);
        DashRootMotionID = 0;
    }

    for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
    {
        if (OriginalMaterials[i]) GetMesh()->SetMaterial(i, OriginalMaterials[i]);
    }

    if (WeaponMesh)
    {
        for (int32 i = 0; i < OriginalWeaponMaterials.Num(); ++i)
        {
            if (OriginalWeaponMaterials[i]) WeaponMesh->SetMaterial(i, OriginalWeaponMaterials[i]);
        }
    }

    GetWorldTimerManager().SetTimer(CooldownTimerHandle, [this]()
    {
        bCanDash = true;
    }, Attribute->DashCooldown, false);

    // El servidor avisa a todos los espectadores que termino el dash
    if (HasAuthority())
        Multicast_StopDashFX();
}

void ANecroLifeCharacter::Multicast_StopDashFX_Implementation()
{
    if (!IsLocallyControlled() && !HasAuthority())
    {
        for (int32 i = 0; i < OriginalMaterials.Num(); ++i)
        {
            if (OriginalMaterials[i]) GetMesh()->SetMaterial(i, OriginalMaterials[i]);
        }
        if (WeaponMesh)
        {
            for (int32 i = 0; i < OriginalWeaponMaterials.Num(); ++i)
            {
                if (OriginalWeaponMaterials[i]) WeaponMesh->SetMaterial(i, OriginalWeaponMaterials[i]);
            }
        }
    }
}

// ============================================================
// Animaciones de combate en red
// ============================================================

void ANecroLifeCharacter::Server_PlayCombatMontage_Implementation(UAnimMontage* MontageToPlay)
{
    Multicast_PlayCombatMontage(MontageToPlay);
}

void ANecroLifeCharacter::Multicast_PlayCombatMontage_Implementation(UAnimMontage* MontageToPlay)
{
    // Solo reproducimos en espectadores para evitar doble reproducción en el dueño
    if (!IsLocallyControlled() && MontageToPlay)
        PlayAnimMontage(MontageToPlay);
}

// ============================================================
// Interacción e inventario
// ============================================================

void ANecroLifeCharacter::Interact()
{
    // No podemos interactuar mientras repartimos golpes
    if (bIsAttacking || bIsDashing || bIsPlunging) return;

    GEngine->AddOnScreenDebugMessage(-1,5.0f, FColor::Yellow, TEXT("Interact"));
    if (CurrentInteractable && CurrentInteractable->Implements<UNecroLifeInterface>())
        INecroLifeInterface::Execute_OnInteract(CurrentInteractable, this);
}

void ANecroLifeCharacter::InventoryInput()
{
    if (!IsLocallyControlled()) return;
    if (bShowForgeInventory)return;

    if (!bShowInventory)
    {      
        bShowInventory = true;
        SetUIState(true);
        Attribute->RecalcularEstadisticas(Inventory->GemsInSlots);
        ShowInventory.Broadcast();
    }
    else
    {
        SetUIState(false);
        bShowInventory = false;
        ShowInventory.Broadcast();
    }
}

void ANecroLifeCharacter::ShowForgeInventoryAction()
{
   
    if (!IsLocallyControlled())
    {
        return;
    }
    ANecroLifePlayerState* PS = Cast<ANecroLifePlayerState>(GetPlayerState());
    PS->GetInventoryComponent()->GemsItems;
    if (!bShowForgeInventory)
    {
        bShowForgeInventory = true;
        SetUIState(true);     
        OnShowForgeInventory.Broadcast(PS->GetInventoryComponent()->GemsItemsInInventory);
    }else
    {
        SetUIState(false);
        bShowForgeInventory=false;
        OnShowForgeInventory.Broadcast(PS->GetInventoryComponent()->GemsItemsInInventory);
    }
}

void ANecroLifeCharacter::TakePosion()
{
    Server_TakePosion();
}

void ANecroLifeCharacter::AddCurrentQuest()
{
    GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
     TEXT("Entro al add current quest"));
     
    if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
  TEXT("Entro al add current quest"));
    
        Server_AgregarMision(NpcBasic->QuestActual);
        Server_NextQuest(NpcBasic); 
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,TEXT("Acepto y entro al addCurrent quest"));
    }  else
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
            TEXT("no entro en el  if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))"));
     
    }  
}

void ANecroLifeCharacter::CancelCurrentQuest()
{
    if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable))
    {
        Server_CancelQuest(NpcBasic); // ← ya lo tenés implementado
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,TEXT("cancelo y entro al Cancelcurrent quest"));
 
    } else
    {
        GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Emerald,
            TEXT("no entro en el  if (ANecroLifeNpcBasic* NpcBasic = Cast<ANecroLifeNpcBasic>(CurrentInteractable) del cancel)"));
     
    }  
}

void ANecroLifeCharacter::Server_CancelQuest_Implementation(ANecroLifeNpcBasic* Npc)
{
    if (Npc)
    {
        Npc->CancelAddQuest(); // ahora corre en el servidor con autoridad
    }
}

void ANecroLifeCharacter::Server_NextQuest_Implementation(ANecroLifeNpcBasic* Npc)
{
    if (Npc)
    {
        Npc->NextAddQuest();
    }
}

bool ANecroLifeCharacter::ShowDialogue(FDialogLine CurrentLine)
{
    if (HubWidget)
    {
        UFunction* Func = HubWidget->FindFunction(FName("ShowDialogueLine"));
        if (Func)
        {
            HubWidget->ProcessEvent(Func, &CurrentLine);
            return CurrentLine.bIsMissionChoice;
        }
    }
    return false;
}

// ============================================================
// UI y misc
// ============================================================

void ANecroLifeCharacter::OnAtributosActualizados(const FEstadisticasPersonaje& NuevosAtributos)
{
    // Ignorar broadcasts con Velocidad = 0 (struct no inicializado / componente secundario vacío)
    if (NuevosAtributos.Velocidad <= 0.f) return;

    GetCharacterMovement()->MaxWalkSpeed = bIsRunning
        ? NuevosAtributos.Velocidad * 200.0f
        : NuevosAtributos.Velocidad * 50.0f;
}

void ANecroLifeCharacter::SetUIState(bool bIsTalking)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {   
        if (bIsTalking)
        {
            PC->SetInputMode(FInputModeGameAndUI());
            PC->bShowMouseCursor = true;
            bCanMove=false;
        }
        else
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
            bCanMove=true;
        }
    }
}

void ANecroLifeCharacter::ShowMsg(FString Msg)
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
}

// ============================================================
// Input bindings
// ============================================================

void ANecroLifeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANecroLifeCharacter::DoJumpStart);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ANecroLifeCharacter::DoJumpEnd);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Move);
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Look);
        EnhancedInputComponent->BindAction(MouseRightDown, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::OnRightMouseDown);
        EnhancedInputComponent->BindAction(MouseRightUp, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::OnRightMouseUp);
        EnhancedInputComponent->BindAction(MouseMiddleDown, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::OnMiddleMouseDown);
        EnhancedInputComponent->BindAction(MouseMiddleUp, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::OnMiddleMouseUp);
        EnhancedInputComponent->BindAction(CameraBoomAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::SetBoomLength);
        EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ANecroLifeCharacter::AbilityEnabled);
        EnhancedInputComponent->BindAction(AbilityCancelAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::AbilityDisambled);
        EnhancedInputComponent->BindAction(OpenInventory, ETriggerEvent::Started, this, &ANecroLifeCharacter::InventoryInput);
        EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Dash);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::Interact);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::RunActivated);
        EnhancedInputComponent->BindAction(ApplyPosion, ETriggerEvent::Triggered, this, &ANecroLifeCharacter::TakePosion);
        EnhancedInputComponent->BindAction(Action, ETriggerEvent::Started, this, &ANecroLifeCharacter::AplyAction);
    }
}

void ANecroLifeCharacter::Server_TakePosion_Implementation()
{
    // ESTO CORRE EN EL SERVIDOR
    if (CachedInventoryComponent)
    {
        // El servidor verifica si hay pociones
        if (CachedInventoryComponent->UseHealtPosion())
        {
            // El servidor aplica la curación
            if (HealthComponent)
            {
                HealthComponent->ApplyHealing(30.0f);
            }
        }
        else
        {
            // Opcional: Cliente recibe mensaje de que no hay pociones
            // (Tendrías que hacer una Client RPC para esto si quieres ser estricto)
        }
    }
}


void ANecroLifeCharacter::ClearWeaponHitMemory()
{
    DamagedActors.Empty();
}

void ANecroLifeCharacter::ApplyWeaponHit(AActor* HitActor)
{
    if (HitActor && HitActor != this && !DamagedActors.Contains(HitActor))
    {
        if (ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(HitActor))
        {
            DamagedActors.Add(HitActor);
            float FinalDamage = Attribute ? Attribute->Attack : 10.0f;
            float Knockback = 0.0f;

            // Verificamos si estamos en medio de una habilidad para aplicar sus modificadores
            if (Ability && Ability->CurrentAbility)
            {
                if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
                {
                    if (AnimInstance->Montage_IsPlaying(Ability->CurrentAbility->AbilityMontage))
                    {
                        FinalDamage *= Ability->CurrentAbility->DamageMultiplier;
                        Knockback = Ability->CurrentAbility->KnockbackForce;
                    }
                }
            }

            // Aplicamos el daño final
            URPGHelper::ApplyDamage(Enemy, FinalDamage);

            // Ejecutamos el Pushback
            // Ejecutamos el Pushback
            if (Knockback > 0.0f)
            {
                FVector KnockbackDirection = Enemy->GetActorLocation() - GetActorLocation();
                
                // Usamos la variable expuesta en lugar del 0.75f hardcodeado
                KnockbackDirection.Z = KnockbackVerticalOffset; 
                
                KnockbackDirection.Normalize();

                Enemy->GetCharacterMovement()->StopMovementImmediately();
                Enemy->StopAnimMontage();
                Enemy->LaunchCharacter(KnockbackDirection * Knockback, true, true);
            }

            // VFX y lógica de muerte
            if (HitVFX) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Enemy->GetActorLocation());
            if (!Enemy->IsAlive())
            {
                URPGHelper::TakeXP(this, Enemy->EsenciasAlMorir);
                Server_ActualizarProgresoMision(Enemy->GetTag(), 1);
            }
        }
    }
}

void ANecroLifeCharacter::EnableAttackRotation()
{
    bCanRotateDuringAttack = true;
}

void ANecroLifeCharacter::DisableAttackRotation()
{
    bCanRotateDuringAttack = false;
}

void ANecroLifeCharacter::ExecuteAreaRoot()
{
    // Ahora toma el valor del Blueprint de Huesos
    float RootRadius = AbilityRootRadius; 
    
    FVector Origin = GetActorLocation();
    
    // --- DEBUG VISUAL --- (La esfera verde durará 2 segundos en pantalla)
    UKismetSystemLibrary::DrawDebugSphere(this, Origin, RootRadius, 12, FLinearColor::Green, 2.f, 2.f);

    TArray<FOverlapResult> Overlaps;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(RootRadius);

    bool bHit = GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, CollisionShape);
    
    Ability->AbilityAply(); 

    if (!bHit) return;

    for (auto& Result : Overlaps)
    {
        AActor* Other = Result.GetActor();
        ANecroLifeEnemyBasic* EnemyBasic = Cast<ANecroLifeEnemyBasic>(Other);
        
        if (!EnemyBasic || Other == this || !EnemyBasic->IsAlive()) continue;

        EnemyBasic->Immobilize(3.0f);
        
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("¡Enemigo %s atrapado!"), *EnemyBasic->GetName()));
    }
}
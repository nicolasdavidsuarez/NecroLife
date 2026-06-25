// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Public/Components/RPGHelper.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ANecroLifeEnemyBasic::ANecroLifeEnemyBasic()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Tu lógica de red
    SetReplicates(true);

    // Componente de salud
    HealthComponent = CreateDefaultSubobject<UUHealthComponent>(TEXT("HealthComponent"));

    // Configuración del TargetWidget (de tus compañeros)
    TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
    TargetWidget->SetupAttachment(RootComponent);
    TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
    TargetWidget->SetDrawSize(FVector2D(64.0f, 64.0f));
    TargetWidget->SetVisibility(false);
}

void ANecroLifeEnemyBasic::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddDynamic(this, &ANecroLifeEnemyBasic::HandleOnHealthChange);
    }

    // Cachear todos los widget components del BP excepto el TargetWidget (targeting indicator)
    GetComponents<UWidgetComponent>(BillboardWidgets);
    BillboardWidgets.Remove(TargetWidget);
}

void ANecroLifeEnemyBasic::HandleOnHealthChange(float Health, float HealthMax)
{
    if (HealthMax > 0)
    {
        float percent = Health / HealthMax;
        BP_SetHealthBar(percent);
    }
}

bool ANecroLifeEnemyBasic::IsAlive()
{
    if (HealthComponent && HealthComponent->IsDead())
    {
        Die(); // Mantenemos tu llamada a la lógica de muerte
        return false;
    }
    return true;
}

void ANecroLifeEnemyBasic::Die()
{
    if (bIsDead || !HasAuthority()) return;
    {
        
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        bIsDead = true;
        OnEnemyDied.Broadcast(this);
        BP_OnDie();
        OnRep_IsDead(); // El servidor también ejecuta la visual localmente
        SetLifeSpan(3.0f);
    }
}

void ANecroLifeEnemyBasic::OnRep_IsDead()
{
    // Tu lógica de Ragdoll replicada
    if (GetMesh())
    {
        GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
        GetMesh()->SetSimulatePhysics(true);
    }
}

void ANecroLifeEnemyBasic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Registro de tu variable bIsDead para el multijugador
    DOREPLIFETIME(ANecroLifeEnemyBasic, bIsDead);
    DOREPLIFETIME(ANecroLifeEnemyBasic, bIsEAttacking);
    DOREPLIFETIME(ANecroLifeEnemyBasic, bIsEDamaged);
}

void ANecroLifeEnemyBasic::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Rotar health bars hacia la cámara local — corre en todos los clientes
    if (BillboardWidgets.Num() > 0)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC && PC->PlayerCameraManager)
        {
            FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
            for (UWidgetComponent* W : BillboardWidgets)
            {
                if (W)
                {
                    FVector Dir = (CamLoc - W->GetComponentLocation()).GetSafeNormal();
                    W->SetWorldRotation(Dir.Rotation());
                }
            }
        }
    }
}

void ANecroLifeEnemyBasic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANecroLifeEnemyBasic::Server_ResetDamageState_Implementation()
{
    bIsEDamaged = false;
}

void ANecroLifeEnemyBasic::ExecuteMeleeAttack()
{
    // ---> TU SOLUCIÓN ACÁ: Si está aturdido, el ataque se cancela inmediatamente
    if (bIsStunned) return;

    // 1. Verificamos autoridad para evitar trampas en red
    if (!HasAuthority() || !GetMesh()) return;

    // DEBUG VITAL: Si este texto no sale en pantalla, el Anim Notify está desconectado.
    UKismetSystemLibrary::PrintString(this, "C++: Disparando ataque", true, true, FLinearColor::Blue, 2.f);

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    TArray<AActor*> OverlappedActors;

    // Radio extendido en Z para cubrir diferencias de altura (rampas, escalones).
    // El filtro real usa Dist2D, igual que la IA para decidir cuándo atacar.
    UKismetSystemLibrary::SphereOverlapActors(
        this,
        GetActorLocation(),
        AttackRange * 1.5f,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappedActors
    );

    for (AActor* HitActor : OverlappedActors)
    {
        if (!HitActor || HitActor->IsA<ANecroLifeEnemyBasic>()) continue;

        if (FVector::Dist2D(GetActorLocation(), HitActor->GetActorLocation()) <= AttackRange)
        {
            UKismetSystemLibrary::PrintString(this, "C++: Le pegue a " + HitActor->GetName(), true, true, FLinearColor::Yellow, 2.f);
            URPGHelper::ApplyDamage(HitActor, AttackDamage);
            break;
        }
    }
}

void ANecroLifeEnemyBasic::Immobilize(float Duration)
{
    if (!IsAlive() || bIsBoss) return;

    bIsStunned = true;

    // Guardamos su velocidad original (si es la primera vez que lo aturden)
    if (DefaultWalkSpeed == 0.0f)
    {
        DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    }

    // PASO CLAVE: Le robamos las piernas. La IA intentará moverse pero su velocidad será 0
    GetCharacterMovement()->MaxWalkSpeed = 0.0f;
    GetCharacterMovement()->StopMovementImmediately();

    if (AController* AIController = GetController())
    {
        AIController->StopMovement();
    }

    // Cortamos la animación de ataque si estaba en medio de una
    StopAnimMontage();

    GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ANecroLifeEnemyBasic::ClearImmobilize, Duration, false);
}

void ANecroLifeEnemyBasic::ClearImmobilize()
{
    bIsStunned = false;

    // Le devolvemos su velocidad original para que vuelva a perseguirte
    if (DefaultWalkSpeed > 0.0f)
    {
        GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
    }
}
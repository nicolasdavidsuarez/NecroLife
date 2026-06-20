// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Net/UnrealNetwork.h" // Necesario para tu replicación
#include "Kismet/KismetSystemLibrary.h"
#include "Public/Components/RPGHelper.h"
#include "Components/WidgetComponent.h"

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

    // Tu suscripción al delegado de salud
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddDynamic(this, &ANecroLifeEnemyBasic::HandleOnHealthChange);
    }
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
    // 1. Verificamos autoridad para evitar trampas
    if (!HasAuthority() || !GetMesh()) return;

    // 2. Obtenemos la posición exacta del socket/hueso
    FVector TraceLocation = GetMesh()->GetSocketLocation(AttackSocketName);

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this); // Que no se golpee a sí mismo

    // NUEVO FIX 1: Ampliamos la búsqueda para asegurarnos de pescar a Huesos
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    TArray<AActor*> OutActors;

    // NUEVO FIX 2: Cambiamos nullptr por AActor::StaticClass()
    bool bHit = UKismetSystemLibrary::SphereOverlapActors(
        this,
        TraceLocation,
        AttackRadius,
        ObjectTypes,
        AActor::StaticClass(), 
        ActorsToIgnore,
        OutActors
    );

    // Dibujamos la esfera manualmente (Verde si toca algo, Roja si no toca nada)
    UKismetSystemLibrary::DrawDebugSphere(
        this,
        TraceLocation,
        AttackRadius,
        12,
        bHit ? FLinearColor::Green : FLinearColor::Red,
        1.5f,
        1.5f
    );

    // 5. Aplicamos daño
    if (bHit)
    {
        for (AActor* HitActor : OutActors)
        {
            if (HitActor)
            {
                // DEBUG: Te imprime en amarillo en la pantalla todo lo que tocó el mordisco
                UKismetSystemLibrary::PrintString(this, "Toque a: " + HitActor->GetName(), true, true, FLinearColor::Yellow, 2.f);

                // Si el objeto tocado NO es un enemigo básico...
                if (!HitActor->IsA<ANecroLifeEnemyBasic>())
                {
                    // DEBUG: Te avisa en verde si la validación funcionó y pegó el golpe
                    UKismetSystemLibrary::PrintString(this, "¡Dano aplicado a " + HitActor->GetName() + "!", true, true, FLinearColor::Green, 2.f);
					
                    URPGHelper::ApplyDamage(HitActor, AttackDamage);
                    break; // Rompemos el ciclo
                }
            }
        }
    }
}
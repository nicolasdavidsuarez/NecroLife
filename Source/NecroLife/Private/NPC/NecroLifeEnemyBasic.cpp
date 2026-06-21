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
    // 1. Verificamos autoridad para evitar trampas en red
    if (!HasAuthority() || !GetMesh()) return;

    // DEBUG VITAL: Si este texto no sale en pantalla, el Anim Notify está desconectado.
    UKismetSystemLibrary::PrintString(this, "C++: Disparando ataque", true, true, FLinearColor::Blue, 2.f);

    // 2. Trayectoria para atravesar a la manada
    FVector TraceStart = GetMesh()->GetSocketLocation(AttackSocketName);
    FVector TraceEnd = TraceStart + (GetActorForwardVector() * 60.f); // Viaja 60 cm hacia adelante

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this); // Que el lobo no se muerda a sí mismo

    // 3. LA SOLUCIÓN: Buscamos específicamente por Tipo de Objeto (Peones)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // Atrapa a Huesos y otros lobos
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    TArray<FHitResult> HitResults;

    // 4. Usamos SphereTraceMultiForObjects
    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        this,
        TraceStart,
        TraceEnd,
        AttackRadius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration, // Dibuja la cápsula
        HitResults,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        1.5f // La cápsula dura 1.5 segundos en pantalla
    );

    // 5. Filtramos los impactos
    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();

            // Si el objeto tocado es válido y NO es otro enemigo básico...
            if (HitActor && !HitActor->IsA<ANecroLifeEnemyBasic>())
            {
                // DEBUG: Te avisa a quién le acaba de pegar
                UKismetSystemLibrary::PrintString(this, "C++: Le pegue a " + HitActor->GetName(), true, true, FLinearColor::Yellow, 2.f);
				
                // Aplicamos daño y rompemos el ciclo
                URPGHelper::ApplyDamage(HitActor, AttackDamage);
                break; 
            }
        }
    }
}
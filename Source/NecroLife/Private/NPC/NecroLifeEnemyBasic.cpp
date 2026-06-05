// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/NPC/NecroLifeEnemyBasic.h"
#include "Net/UnrealNetwork.h" // Necesario para tu replicación
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
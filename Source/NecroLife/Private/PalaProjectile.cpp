#include "PalaProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "NPC/NecroLifeEnemyBasic.h"
#include "Public/Components/RPGHelper.h"

// ============================================================
// Constructor
// ============================================================
APalaProjectile::APalaProjectile()
{
    PrimaryActorTick.bCanEverTick = false; // No necesitamos Tick, optimizamos rendimiento

    // 1. Colisión Principal (Esfera)
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(40.0f);
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComp;

    // 2. Malla Visual (La pala)
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // La malla es solo visual

    // 3. Movimiento del Proyectil (Vuelo recto)
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 1500.f; // Velocidad de salida
    ProjectileMovement->MaxSpeed = 1500.f;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // Cero gravedad para que no caiga
    
    // 4. Movimiento de Rotación (Giro estilo shuriken)
    RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
    // Giramos sobre el eje Pitch, Yaw o Roll (dependerá de cómo esté orientada tu malla original)
    // Probamos con Yaw (Z). Si la pala gira raro en el juego, cambiamos este vector a (1000, 0, 0) o (0, 1000, 0).
    RotatingMovement->RotationRate = FRotator(0.f, 1500.f, 0.f); 

    // Destrucción automática a los 3 segundos para limpiar la memoria si no choca con nada
    InitialLifeSpan = 3.0f;
}

// ============================================================
// Begin Play
// ============================================================
void APalaProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    // Conectamos el evento de impacto
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APalaProjectile::OnProjectileOverlap);
}

// ============================================================
// Lógica de Impacto
// ============================================================
void APalaProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Verificamos que toquemos algo válido, que no seamos nosotros mismos, que no sea Huesos (el instigador) y que no lo hayamos golpeado ya
    if (OtherActor && OtherActor != this && OtherActor != GetInstigator() && !DamagedActors.Contains(OtherActor))
    {
        // Si el objeto tocado es un enemigo...
        if (ANecroLifeEnemyBasic* Enemy = Cast<ANecroLifeEnemyBasic>(OtherActor))
        {
            // Lo guardamos en memoria para atravesarlo y no volver a dañarlo
            DamagedActors.Add(OtherActor);

            // Aplicamos el daño
            URPGHelper::ApplyDamage(Enemy, DamageAmount);
        }
    }
}


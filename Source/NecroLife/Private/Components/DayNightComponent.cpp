
#include "Components/DayNightComponent.h"

#include "Chaos/ChaosPerfTest.h"
#include "Kismet/KismetMathLibrary.h"

UDayNightComponent::UDayNightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDayNightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDayNightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime += DeltaTime / DayLength;
	if (CurrentTime > 1.f) CurrentTime -= 1.f;
	if (bAutoCycle)
	{
		CurrentTime += DeltaTime / DayLength;
		if (CurrentTime > 1.f) CurrentTime -= 1.f;

		UpdateLighting();
		//cosas que suceden el dia/noche se actualizan aca se mandan delegados
	}
}

void UDayNightComponent::UpdateLighting()
{
	if (!SunLight || !SkyLight) return;

	// Rotar Sol y Luna
	float SunAngle = CurrentTime * 360.f;
	//SunAngle = FMath::Clamp(SunAngle, 181.f, 359.f);

	
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("Sun Angle: %.2f"), SunAngle));
	}
	FRotator CurrentRotation = SunLight->GetActorRotation();
	
	SunLight->SetActorRotation(FRotator(SunAngle, CurrentRotation.Yaw,CurrentRotation.Roll));
		
	}





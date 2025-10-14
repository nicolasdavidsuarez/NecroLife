#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/AudioComponent.h"
#include "DayNightComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NECROLIFE_API UDayNightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDayNightComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Luz principal del sol */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	ADirectionalLight* SunLight;

	/** Luz ambiental global */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	ASkyLight* SkyLight;

	/** Duración de un día completo en segundos */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	float DayLength = 300.f;

	/** Hora actual del día: 0-1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	float CurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DayNight")
	bool bAutoCycle = true;

private:
	void UpdateLighting();
	
};

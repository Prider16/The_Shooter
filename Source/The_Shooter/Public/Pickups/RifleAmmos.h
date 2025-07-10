// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleAmmos.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class THE_SHOOTER_API ARifleAmmos : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	// Amplitude of hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hover", meta = (AllowPrivateAccess = "true"))
	float HoverHeight;

	// Speed of hover
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hover", meta = (AllowPrivateAccess = "true"))
	float HoverSpeed;

public:	
	// Sets default values for this actor's properties
	ARifleAmmos();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	// For animation timing
	float RunningTime;
};

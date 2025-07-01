// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class THE_SHOOTER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
	/*Skeleton Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;

	


public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	float Pistol_totalammo = 30.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	float Pistol_currentammo = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	float Rifle_totalammo = 60.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	float Rifle_currentammo = 30.f;

	// Pistol Actions
	void Pistol_Fire();
	void Pistol_Reload();

	// Rifle Actions
	void Rifle_Fire();
	void Rifle_Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

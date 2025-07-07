// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h" 
#include "WeaponBase.generated.h"

//class UNiagaraSystem;

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

	// Variables
	// Pistol
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	int32 Pistol_totalammo = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	int32 Pistol_currentammo = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variables)
	int32 Pistol_maxammo = 15.f;

	// Rifle
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	int32 Rifle_totalammo = 60.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	int32 Rifle_currentammo = 30.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variables)
	int32 Rifle_maxammo = 30.f;

	// Firing 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variables)
	bool Firing = false;

	// Pistol Actions
	void Pistol_Fire();
	void Pistol_Reload();

	// Rifle Actions
	void Rifle_Fire();
	void Rifle_Reload();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* GetWeaponMesh() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ParticleEffect)
	UNiagaraSystem* ParticleToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Damage that will be applied to other
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Pistol_Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Rifle_Damage = 30.0f;

private:

	// Function to Perform Line Trace
	void PerformLineTrace(float Damage);

};

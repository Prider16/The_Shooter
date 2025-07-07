// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletonMesh"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);

}

void AWeaponBase::Pistol_Fire()
{
	Firing = true;
	if (Pistol_currentammo <= 0) 
	{ 
		// If we have 0 ammo then we can't shoot, We need to Reload
		UE_LOG(LogTemp, Warning, TEXT("Reload!"));
		return;
	};

	Pistol_currentammo--;

	PerformLineTrace(Pistol_Damage);

}

void AWeaponBase::Pistol_Reload()
{
	if (Pistol_totalammo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of Ammo!"));
		return;
	}
	if (Pistol_currentammo == Pistol_totalammo)
	{
		// If we have full mag then we can't reload
		UE_LOG(LogTemp, Warning, TEXT("Mag is fully Loaded"));
		return;
	}
	
	float TempAmmo = Pistol_maxammo - Pistol_currentammo;

	Pistol_currentammo = Pistol_currentammo + TempAmmo;

	Pistol_totalammo = Pistol_totalammo - TempAmmo;

}

void AWeaponBase::Rifle_Fire()
{
	Firing = true;
	if (Rifle_currentammo <= 0)
	{
		// If we have 0 ammo then we can't shoot, We need to Reload
		UE_LOG(LogTemp, Warning, TEXT("Reload!"));
		return;
	};

	Rifle_currentammo--;

	PerformLineTrace(Rifle_Damage);
}

void AWeaponBase::Rifle_Reload()
{
	if (Rifle_totalammo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of Ammo!"));
		return;
	}

	if (Rifle_currentammo == Rifle_totalammo)
	{
		// If we have full mag then we can't reload
		UE_LOG(LogTemp, Warning, TEXT("Mag is fully Loaded"));
		return;
	}
	float TempAmmo = Rifle_maxammo - Rifle_currentammo;

	Rifle_currentammo = Rifle_currentammo + TempAmmo;

	Rifle_totalammo = Rifle_totalammo - TempAmmo;
}

void AWeaponBase::PerformLineTrace(float Damage)
{
	// Get the player Controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;

	// Getting the player Viewpoint
	FVector Location;
	FRotator Rotation;
	PlayerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * 20000.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Ignore weapon

	// Do linetrace and store in bool to check if we did hit or not
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, Params);

	// we linetrace hit something...
	if (bHit)
	{

		/*if (ParticleToSpawn)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ParticleToSpawn,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation()
			);
		}*/

		// ... We get the hit actor
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// print the hitActor name in the Log
			UE_LOG(LogTemp, Log, TEXT("Hit %s "), *HitActor->GetName());
		}
	}

	// Debug line
	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2.0f, 0, 1.0f);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* AWeaponBase::GetWeaponMesh() const
{
	return WeaponSkeletalMesh;
}


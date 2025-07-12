// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/HealthBandages.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "D:\Unreal Project\The_Shooter\Source\The_Shooter\The_ShooterCharacter.h"

// Sets default values
AHealthBandages::AHealthBandages()
{
	PrimaryActorTick.bCanEverTick = true;

	BandageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BandageMesh"));
	SetRootComponent(BandageMesh);

	BandageMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(BandageMesh);
	CollisionSphere->InitSphereRadius(200.f);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));

	// Set default values
	HoverHeight = 20.0f;
	HoverSpeed = 6.0f;
	RunningTime = 0.0f;

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHealthBandages::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHealthBandages::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthBandages::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	// Calculate vertical offset using sine wave
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += FMath::Sin(RunningTime * HoverSpeed) * HoverHeight * DeltaTime;

	SetActorLocation(NewLocation);

}

void AHealthBandages::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AThe_ShooterCharacter* Player = Cast<AThe_ShooterCharacter>(OtherActor);
		if (Player)
		{
			Player->AddBandages(1);
			Destroy();
		}
	}
}


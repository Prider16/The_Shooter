// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PistolAmmos.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "D:\Unreal Project\The_Shooter\Source\The_Shooter\The_ShooterCharacter.h"


// Sets default values
APistolAmmos::APistolAmmos()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BandageMesh"));
	SetRootComponent(AmmoMesh);

	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(AmmoMesh);
	CollisionSphere->InitSphereRadius(200.f);
	CollisionSphere->SetCollisionProfileName(TEXT("Trigger"));

	// Set default values
	HoverHeight = 20.0f;
	HoverSpeed = 6.0f;
	RunningTime = 0.0f;

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APistolAmmos::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APistolAmmos::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolAmmos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	// Calculate vertical offset using sine wave
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += FMath::Sin(RunningTime * HoverSpeed) * HoverHeight * DeltaTime;

	SetActorLocation(NewLocation);
}

void APistolAmmos::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AThe_ShooterCharacter* Player = Cast<AThe_ShooterCharacter>(OtherActor);
		if (Player)
		{
			Player->AddPistolAmmos(15);
			Destroy();
		}
	}
}


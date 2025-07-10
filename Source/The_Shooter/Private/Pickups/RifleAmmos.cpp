// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/RifleAmmos.h"

// Sets default values
ARifleAmmos::ARifleAmmos()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARifleAmmos::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifleAmmos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


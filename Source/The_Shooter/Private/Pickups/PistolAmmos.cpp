// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PistolAmmos.h"

// Sets default values
APistolAmmos::APistolAmmos()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}


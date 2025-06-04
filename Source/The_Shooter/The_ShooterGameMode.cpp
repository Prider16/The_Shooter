// Copyright Epic Games, Inc. All Rights Reserved.

#include "The_ShooterGameMode.h"
#include "The_ShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThe_ShooterGameMode::AThe_ShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

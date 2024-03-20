// Copyright Epic Games, Inc. All Rights Reserved.

#include "Odyssey_Of_Tycho__GameMode.h"
#include "Odyssey_Of_Tycho__Character.h"
#include "UObject/ConstructorHelpers.h"

AOdyssey_Of_Tycho__GameMode::AOdyssey_Of_Tycho__GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "AI_DemoGameMode.h"
#include "AI_DemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAI_DemoGameMode::AAI_DemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

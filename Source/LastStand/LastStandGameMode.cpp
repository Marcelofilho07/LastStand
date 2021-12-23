// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastStandGameMode.h"
#include "LastStandHUD.h"
#include "LastStandCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALastStandGameMode::ALastStandGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALastStandHUD::StaticClass();
}

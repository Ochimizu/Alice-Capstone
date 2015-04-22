// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "AliceGameMode.h"

AAliceGameMode::AAliceGameMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer) 
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Alice/Alice"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Set the default HUD class
	static ConstructorHelpers::FClassFinder<AHUD> HudBPClass(TEXT("/Game/HUD/TestHUD"));
	if (HudBPClass.Class != NULL) {
		HUDClass = HudBPClass.Class;
	}
}
// Copyright Epic Games, Inc. All Rights Reserved.

#include "WeaponBounceGameMode.h"
#include "WeaponBounceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWeaponBounceGameMode::AWeaponBounceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

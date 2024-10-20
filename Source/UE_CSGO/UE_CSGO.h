// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class e_weapon_animation_type : uint8
{
	knife	UMETA(DisplayName = "knife"),
	beretta	UMETA(DisplayName = "beretta"),
	ak47	UMETA(DisplayName = "ak47")
};

UENUM(BlueprintType)
enum class e_weapon_type : uint8
{
	knife	UMETA(DisplayName = "knife"),
	pistol	UMETA(DisplayName = "pistol"),
	rifle	UMETA(DisplayName = "rifle")
};


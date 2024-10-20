// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/character_master.h"
#include "GameFramework/PlayerController.h"
#include "pc_ue_csgo.generated.h"

/**
 * 
 */
UCLASS()
class UE_CSGO_API Apc_ue_csgo : public APlayerController
{
	GENERATED_BODY()
public:
	void spawn_player(TSubclassOf<AActor> team, FVector location, FRotator rotation);
private:
	Acharacter_master* player_character;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "pc_ue_csgo.h"

#include "Character/character_master.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void Apc_ue_csgo::spawn_player(TSubclassOf<AActor> team, FVector location, FRotator rotation)
{
	player_character = GetWorld()->SpawnActor<Acharacter_master>(team, location, rotation);
	if (player_character)
	{
		Possess(player_character);
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_CSGOGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "spawn_points/player_start_base.h"

AUE_CSGOGameModeBase::AUE_CSGOGameModeBase()
{
    PlayerControllerClass = Apc_ue_csgo::StaticClass();
    // Chargement des Blueprints pour les points de spawn
    static ConstructorHelpers::FClassFinder<Aplayer_start_base> bp_counter_terrorist_spawn_point_actor(TEXT("/Game/_Main/Blueprints/SpawnPoints/bp_counter_terrorists_spawn_point.bp_counter_terrorists_spawn_point_C"));
    static ConstructorHelpers::FClassFinder<Aplayer_start_base> bp_terrorist_spawn_point_actor(TEXT("/Game/_Main/Blueprints/SpawnPoints/bp_terrorist_spawn_point.bp_terrorist_spawn_point_C"));
    static ConstructorHelpers::FClassFinder<Acharacter_master> bp_terrorist_actor(TEXT("/Game/_Main/Blueprints/Character/bp_terrorist.bp_terrorist_C"));
    static ConstructorHelpers::FClassFinder<Acharacter_master> bp_counter_terrorist_actor(TEXT("/Game/_Main/Blueprints/Character/bp_counter_terrorist.bp_counter_terrorist_C"));

    if (bp_counter_terrorist_spawn_point_actor.Class != nullptr)
    {
        bp_counter_terrorist_spawn_point = bp_counter_terrorist_spawn_point_actor.Class;
    }
    if (bp_terrorist_spawn_point_actor.Class != nullptr)
    {
        bp_terrorist_spawn_point = bp_terrorist_spawn_point_actor.Class;
    }
    if (bp_terrorist_actor.Class != nullptr)
    {
        bp_terrorist = bp_terrorist_actor.Class;
    }
    if (bp_counter_terrorist_actor.Class != nullptr)
    {
        bp_counter_terrorist = bp_counter_terrorist_actor.Class;
    }
}

void AUE_CSGOGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    // Ajouter le nouveau joueur à la liste des joueurs connectés
    get_connected_players().AddUnique(NewPlayer);

    // Récupérer tous les acteurs des classes Blueprint pour les points de spawn
    if (bp_counter_terrorist_spawn_point)
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), bp_counter_terrorist_spawn_point, counter_terrorist_spawn_points);
    }

    if (bp_terrorist_spawn_point)
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), bp_terrorist_spawn_point, terrorist_spawn_points);
    }

    // Vérifier qu'il y a au moins un point de spawn terroriste avant d'y accéder
    if (terrorist_spawn_points.Num() > 0 && terrorist_spawn_points[0])
    {
        for (AActor* Actor : terrorist_spawn_points)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *Actor->GetName());
        }
    }
    if (counter_terrorist_spawn_points.Num() > 0 && counter_terrorist_spawn_points[0])
    {
        for (AActor* Actor : counter_terrorist_spawn_points)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *Actor->GetName());
        }
    }
    Apc_ue_csgo* pc_ue_csgo = Cast<Apc_ue_csgo>(NewPlayer);
    if (pc_ue_csgo)
    {
        request_spawn_player(pc_ue_csgo);
    }
}

void AUE_CSGOGameModeBase::request_spawn_player(Apc_ue_csgo* player_controller)
{
    TArray<TSubclassOf<AActor>> teams = {bp_terrorist, bp_counter_terrorist};

    TSubclassOf<AActor> player_team;
    if (counter_terrorists < 5)
    {
        player_team = bp_counter_terrorist;
        counter_terrorists++;
    }
    else
    {
        player_team = bp_terrorist;
    }
    if (player_team == bp_counter_terrorist)
    {
        FTransform spawn_transform = counter_terrorist_spawn_points[FMath::RandRange(0, counter_terrorist_spawn_points.Num() - 1)]->GetActorTransform();
        player_controller->spawn_player(player_team, spawn_transform.GetTranslation(), spawn_transform.GetRotation().Rotator());
    }
    else
    {
        FTransform spawn_transform = terrorist_spawn_points[FMath::RandRange(0, terrorist_spawn_points.Num() - 1)]->GetActorTransform();
        player_controller->spawn_player(player_team, spawn_transform.GetTranslation(), spawn_transform.GetRotation().Rotator());
    }
}

// Copyright Epic Games, Inc. All Rights Reserved.

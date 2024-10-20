// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "pc_ue_csgo.h"
#include "Character/character_master.h"
#include "GameFramework/GameModeBase.h"
#include "UE_CSGOGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_CSGO_API AUE_CSGOGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	AUE_CSGOGameModeBase();
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
private:
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TArray<APlayerController*> connected_players;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TArray<AActor*> counter_terrorist_spawn_points;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TArray<AActor*> terrorist_spawn_points;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> bp_counter_terrorist_spawn_point;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> bp_terrorist_spawn_point;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> bp_counter_terrorist;
	UPROPERTY(BlueprintReadOnly, Category = "Players Informations", meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> bp_terrorist;
	int counter_terrorists;
	
protected:
	void request_spawn_player(Apc_ue_csgo* player_controller);
public:
	TArray<APlayerController*>& get_connected_players() { return connected_players; }
	
	
};

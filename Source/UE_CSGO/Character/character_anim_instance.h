// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "character_master.h"
#include "Animation/AnimInstance.h"
#include "character_anim_instance.generated.h"

/**
 * 
 */
UCLASS()
class UE_CSGO_API Ucharacter_anim_instance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float get_movement_speed() const {  return movement_speed;  };
	
	void set_movement_speed(float val) { movement_speed = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	float get_movement_direction() const {  return movement_direction;  };
	
	void set_movement_direction(float val) { movement_direction = val; };
	
	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool get_is_in_air() const {  return b_is_in_air;  };
	
	void set_is_in_air(bool val) { b_is_in_air = val; };

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	TArray<e_movement_state>& get_movement_state() {  return movement_state; };

	void set_movement_state(TArray<e_movement_state>& val) { movement_state = val; };
private:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void UpdateAnimationProperties(float DeltaTime);
	float	movement_speed;
	float	movement_direction;
	TArray<e_movement_state> movement_state;
	bool b_is_in_air;
};

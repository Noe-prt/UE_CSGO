// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "UE_CSGO/UE_CSGO.h"
#include "GameFramework/SpringArmComponent.h"
#include "UE_CSGO/weapon/weapon_master.h"
#include "character_master.generated.h"

UENUM(BlueprintType)
enum class e_movement_state : uint8
{
	running     UMETA(DisplayName = "running"),
	walking		UMETA(DisplayName = "walking"),
	crouching     UMETA(DisplayName = "crouching"),
};

UCLASS()
class UE_CSGO_API Acharacter_master : public ACharacter
{
	GENERATED_BODY()

public:
	Acharacter_master();
	
protected:
	virtual void BeginPlay() override;
	
	void	move_forward(float value);
	void	move_right(float value);
	void	turn(float value);
	void	look_up(float value);
	void	crouch_input_pressed();
	void	crouch_input_released();
	void	walk_input_pressed();
	void	walk_input_released();
	void	jump_input_pressed();
	void	walk(bool b_walk_local);
	void	crouch(bool b_crouch_local);
	void calculate_aim_direction();
	UFUNCTION(Server, Reliable)
	void	server_crouch(bool b_crouch_local);
	UFUNCTION(Server, Reliable)
	void	server_walk(bool b_walk_local);
	UFUNCTION(Server, Reliable)
	void server_add_movement_state(e_movement_state new_state);
	UFUNCTION(Server, Reliable)
	void server_calculate_aim_direction();
	void delay(float duration, FName function_name);
	void weapon_sway(float delta_time);
	void arms_movement(float delta_time);
	void play_footsteps_sfx();
	UFUNCTION(Server, Reliable)
	void srv_play_footsteps_sfx();
	UFUNCTION(NetMulticast, Reliable)
	void mc_play_footsteps_sfx();
	void update_footsteps();
	UFUNCTION(Server, Reliable)
	void srv_loot_weapon(Aweapon_master* weapon);
	UFUNCTION()
	void OnRep_equipped_weapon();
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent*		spring_arm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent*		fp_camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent*		fp_mesh;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TArray<e_movement_state>	movement_state;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	e_weapon_type	equipped_weapon_type;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	e_weapon_animation_type	equipped_weapon_animation_type;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float	aim_direction;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	float	weapon_pitch;
    
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	float	weapon_yaw;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway", meta = (AllowPrivateAccess = "true"))
	float	sway_amount;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Sway", meta = (AllowPrivateAccess = "true"))
	float	sway_speed;
    
	UPROPERTY(EditAnywhere, Category = "Weapon Sway", meta = (AllowPrivateAccess = "true"))
	float	max_sway_angle;

	UPROPERTY(EditAnywhere, Category = "Arms movement", meta = (AllowPrivateAccess = "true"))
	FVector arm_offset;
	
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	FVector original_arm_pos;
	
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	FTimerHandle footsteps_timer_handle;

	UPROPERTY(EditAnywhere, Category = "Footsteps", meta = (AllowPrivateAccess = "true"))
	USoundBase* footsteps_cue;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	bool	playing_footsteps_sfx;

	UPROPERTY(Replicated, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<Aweapon_master*>	weapon_inventory;
	
	UPROPERTY(meta = (AllowPrivateAccess = "true"))
	bool	b_can_loot_weapon;

	UPROPERTY(ReplicatedUsing = OnRep_equipped_weapon, meta = (AllowPrivateAccess = "true"))
	Aweapon_master*		equipped_weapon;
public:
	USpringArmComponent* get_spring_arm() const {	return spring_arm; };
	UCameraComponent* get_fp_camera() const {	return fp_camera; };
	USkeletalMeshComponent* get_fp_mesh() const {	return fp_mesh; };
	TArray<e_movement_state>& get_movement_state() {  return movement_state; };
	float get_aim_direction() const {  return aim_direction; };
	void set_aim_direction(float val) {aim_direction = val;}
	e_weapon_type get_equipped_weapon_type() const {  return equipped_weapon_type;  };
	void	set_equipped_weapon_type(e_weapon_type weapon_type) {equipped_weapon_type = weapon_type;};
	e_weapon_animation_type get_equipped_weapon_animation_type() const {  return equipped_weapon_animation_type;  };
	void	set_equipped_weapon_animation_type(e_weapon_animation_type weapon_animation_type) {equipped_weapon_animation_type = weapon_animation_type;};
	TArray<Aweapon_master*> get_weapon_inventory() const {  return weapon_inventory;  };
	bool get_can_loot_weapon() const {return b_can_loot_weapon;};
	void set_can_loot_weapon(bool local_can_loot_weapon) {b_can_loot_weapon = local_can_loot_weapon;};
	void loot_weapon(Aweapon_master *weapon);
	
};

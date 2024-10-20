// Fill out your copyright notice in the Description page of Project Settings.


#include "character_master.h"

#include "character_anim_instance.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

Acharacter_master::Acharacter_master()
{
	PrimaryActorTick.bCanEverTick = true;
	spring_arm = CreateDefaultSubobject<USpringArmComponent>("spring_arm");
	fp_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("fp_camera"));
	fp_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("fp_mesh"));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -95.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetOwnerNoSee(true);
	
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 78.f;
	GetCharacterMovement()->JumpZVelocity = 300.f;
	
	get_spring_arm()->SetupAttachment(RootComponent);
	get_spring_arm()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 65.f), FRotator(0.f, 90.f, 0.f));
	get_spring_arm()->bUsePawnControlRotation = true;
	get_spring_arm()->TargetArmLength = 0.f;
	get_spring_arm()->bEnableCameraLag;
	get_spring_arm()->CameraLagSpeed = 10.f;
	
	GetCapsuleComponent()->SetCapsuleRadius(50.f);
	
	get_fp_camera()->SetupAttachment(get_spring_arm());
	
	get_fp_mesh()->SetupAttachment(get_fp_camera());
	get_fp_mesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -165.5f), FRotator(0.f, -90.f, 0.f));
	get_fp_mesh()->SetOnlyOwnerSee(true);
	
	get_movement_state().Add(e_movement_state::running);
	set_equipped_weapon_type(e_weapon_type::knife);
	set_equipped_weapon_animation_type(e_weapon_animation_type::knife);
	
	sway_amount = 3.f;
	sway_speed = 5.f;
	max_sway_angle = 10.f;
	arm_offset = FVector(-4.f, 0.f, -1.f);

	Tags.Add("Player");

	set_can_loot_weapon(true);
}

void Acharacter_master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Acharacter_master, movement_state);
	DOREPLIFETIME(Acharacter_master, aim_direction);
	DOREPLIFETIME(Acharacter_master, weapon_inventory);
	DOREPLIFETIME(Acharacter_master, equipped_weapon);
	DOREPLIFETIME(Acharacter_master, equipped_weapon_type);
	DOREPLIFETIME(Acharacter_master, equipped_weapon_animation_type);
}

void Acharacter_master::BeginPlay()
{
	Super::BeginPlay();
	original_arm_pos = fp_mesh->GetRelativeLocation();
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void Acharacter_master::delay(float duration, FName function_name)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = function_name;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	UKismetSystemLibrary::Delay(this, duration, LatentInfo);
}

void Acharacter_master::update_footsteps()
{
	bool b_is_moving = GetVelocity().Size() > 285.f ? true : false;

	if (b_is_moving && !GetCharacterMovement()->IsFalling() && !playing_footsteps_sfx)
	{
		play_footsteps_sfx();
		playing_footsteps_sfx = true;
		GetWorld()->GetTimerManager().SetTimer(
			footsteps_timer_handle,
			this,
			&Acharacter_master::play_footsteps_sfx,
			0.34f,
			true
		);
	}
	else if ((!b_is_moving || GetCharacterMovement()->IsFalling()) && playing_footsteps_sfx)
	{
		GetWorld()->GetTimerManager().ClearTimer(footsteps_timer_handle);
		playing_footsteps_sfx = false;
	}
}

void Acharacter_master::loot_weapon(Aweapon_master* weapon)
{
	srv_loot_weapon(weapon);
}

void Acharacter_master::srv_loot_weapon_Implementation(Aweapon_master* weapon)
{
	weapon_inventory.Add(weapon);
	equipped_weapon = weapon;
	OnRep_equipped_weapon();
	set_equipped_weapon_type(equipped_weapon->get_weapon_type());
	set_equipped_weapon_animation_type(equipped_weapon->get_weapon_animation_type());
}

void Acharacter_master::OnRep_equipped_weapon()
{
	equipped_weapon->SetOwner(this);
	equipped_weapon->get_weapon_mesh()->SetSimulatePhysics(false);
	equipped_weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, equipped_weapon->get_weapon_socket());
	equipped_weapon->get_weapon_sphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void Acharacter_master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	weapon_sway(DeltaTime);
	arms_movement(DeltaTime);
}

void Acharacter_master::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &Acharacter_master::move_forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Acharacter_master::move_right);
	PlayerInputComponent->BindAxis("Turn", this, &Acharacter_master::turn);
	PlayerInputComponent->BindAxis("LookUp", this, &Acharacter_master::look_up);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &Acharacter_master::crouch_input_pressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &Acharacter_master::crouch_input_released);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &Acharacter_master::walk_input_pressed);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &Acharacter_master::walk_input_released);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &Acharacter_master::jump_input_pressed);

}

void Acharacter_master::weapon_sway(float delta_time)
{
	float mouse_delta_x, mouse_delta_y;
	GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(mouse_delta_x, mouse_delta_y);
	weapon_pitch += mouse_delta_y * sway_amount * delta_time;
	weapon_yaw += mouse_delta_x * sway_amount * delta_time;
	weapon_pitch = FMath::Clamp(weapon_pitch, -max_sway_angle, max_sway_angle);
	weapon_yaw = FMath::Clamp(weapon_yaw, -max_sway_angle, max_sway_angle);
	weapon_pitch = FMath::FInterpTo(weapon_pitch, 0.0f, delta_time, sway_speed);
	weapon_yaw = FMath::FInterpTo(weapon_yaw, 0.0f, delta_time, sway_speed);
	FRotator NewRotation = FRotator(weapon_pitch, weapon_yaw - 90.f, 0.0f);
	fp_mesh->SetRelativeRotation(NewRotation);
}

void Acharacter_master::arms_movement(float delta_time)
{
	float forward_input = GetInputAxisValue("MoveForward");
	float right_input = GetInputAxisValue("MoveRight");
	bool b_is_moving = FMath::Abs(forward_input) > 0.f || FMath::Abs(right_input) > 0.f;
	FVector target_pos = b_is_moving ? original_arm_pos + arm_offset : original_arm_pos;
	FVector new_pos = FMath::VInterpTo(get_fp_mesh()->GetRelativeLocation(), target_pos, delta_time, sway_speed);
	get_fp_mesh()->SetRelativeLocation(new_pos);
}

void Acharacter_master::play_footsteps_sfx()
{
	srv_play_footsteps_sfx();
}

void Acharacter_master::srv_play_footsteps_sfx_Implementation()
{
	mc_play_footsteps_sfx();
}

void Acharacter_master::mc_play_footsteps_sfx_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, footsteps_cue, GetActorLocation());
}

void Acharacter_master::move_forward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
	update_footsteps();
}

void Acharacter_master::move_right(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
	update_footsteps();
}

void Acharacter_master::turn(float value)
{
	AddControllerYawInput(value);
}

void Acharacter_master::look_up(float value)
{
	AddControllerPitchInput(value);
	calculate_aim_direction();
}

void Acharacter_master::crouch_input_pressed()
{
	if (!GetCharacterMovement()->IsFalling())
		crouch(true);
	else
	{
		delay(0.05f, "crouch_input_pressed");
	}
}
void Acharacter_master::crouch_input_released()
{
	if (get_movement_state().Contains(e_movement_state::crouching))
		crouch(false);
}

void Acharacter_master::crouch(bool b_crouch_local)
{
	if (HasAuthority())
	{
		if (b_crouch_local)
		{
			Crouch();
			get_movement_state().Add(e_movement_state::crouching);
		}
		else
		{
			UnCrouch();
			get_movement_state().Remove(e_movement_state::crouching);
		}
	}
	else
	{
		server_crouch(b_crouch_local);
		if (b_crouch_local)
		{
			Crouch();
			get_movement_state().Add(e_movement_state::crouching);
		}
		else
		{
			UnCrouch();
			get_movement_state().Remove(e_movement_state::crouching);
		}
	}
}

void Acharacter_master::calculate_aim_direction()
{
	if (!HasAuthority())
		server_calculate_aim_direction();
	FRotator ControlRot = GetControlRotation();
	FRotator ActorRot = GetActorRotation();
	FRotator AimRot = ControlRot - ActorRot;
	AimRot.Normalize();
	set_aim_direction(AimRot.Pitch);
	
}

void Acharacter_master::walk_input_pressed()
{
	walk(true);
}

void Acharacter_master::walk_input_released()
{
	walk(false);
}

void Acharacter_master::jump_input_pressed()
{
	if (!GetCharacterMovement()->IsFalling())
		Jump();
}

void Acharacter_master::walk(bool b_walk_local)
{
	if (HasAuthority())
	{
		if (b_walk_local)
		{
			GetCharacterMovement()->MaxWalkSpeed = 156.f;
			get_movement_state().Add(e_movement_state::walking);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			get_movement_state().Remove(e_movement_state::walking);
		}
	}
	else
	{
		server_walk(b_walk_local);
		if (b_walk_local)
		{
			GetCharacterMovement()->MaxWalkSpeed = 156.f;
			get_movement_state().Add(e_movement_state::walking);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			get_movement_state().Remove(e_movement_state::walking);
		}
	}
}

void Acharacter_master::server_crouch_Implementation(bool b_crouch_local)
{
	crouch(b_crouch_local);
}

void Acharacter_master::server_walk_Implementation(bool b_walk_local)
{
	walk(b_walk_local);
}

void Acharacter_master::server_add_movement_state_Implementation(e_movement_state new_state)
{
	movement_state.Add(new_state);
}

void Acharacter_master::server_calculate_aim_direction_Implementation()
{
	calculate_aim_direction();
}


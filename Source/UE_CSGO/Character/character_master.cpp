// Fill out your copyright notice in the Description page of Project Settings.


#include "character_master.h"

#include "character_anim_instance.h"
#include "NavigationSystemTypes.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
Acharacter_master::Acharacter_master()
{
	PrimaryActorTick.bCanEverTick = true;
	spring_arm = CreateDefaultSubobject<USpringArmComponent>("spring_arm");
	fp_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("fp_camera"));
	fp_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("fp_mesh"));

	// third person mesh settings
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -95.f), FRotator(0.f, -90.f, 0.f));

	// Movements settings
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 78.f;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->JumpZVelocity = 300.f;

	// spring arm settings
	get_spring_arm()->SetupAttachment(GetMesh());
	get_spring_arm()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 140.f), FRotator(0.f, 90.f, 0.f));
	get_spring_arm()->bUsePawnControlRotation = true;
	get_spring_arm()->TargetArmLength = 300.f;

	// camera settings
	get_fp_camera()->SetupAttachment(get_spring_arm());

	// first person mesh settings
	get_fp_mesh()->SetupAttachment(get_fp_camera());

	get_movement_state().Add(e_movement_state::running);
}

void Acharacter_master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Acharacter_master, movement_state);
}

// Called when the game starts or when spawned
void Acharacter_master::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
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

void Acharacter_master::move_forward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void Acharacter_master::move_right(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void Acharacter_master::turn(float value)
{
	AddControllerYawInput(value);
}

void Acharacter_master::look_up(float value)
{
	AddControllerPitchInput(value);
}

void Acharacter_master::crouch_input_pressed()
{
	crouch(true);
}

void Acharacter_master::crouch_input_released()
{
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

// Called every frame
void Acharacter_master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


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
	GetCharacterMovement()->MaxWalkSpeedCrouched = 50.f;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 156.f;

	// spring arm settings
	spring_arm->SetupAttachment(GetMesh());
	spring_arm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 140.f), FRotator(0.f, 90.f, 0.f));
	spring_arm->bUsePawnControlRotation = true;
	spring_arm->TargetArmLength = 300.f;

	// camera settings
	fp_camera->SetupAttachment(spring_arm);

	// first person mesh settings
	fp_mesh->SetupAttachment(fp_camera);
}

void Acharacter_master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Acharacter_master, b_crouch);
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
			b_crouch = true;
		}
		else
		{
			UnCrouch();
			b_crouch = false;
		}
	}
	else
	{
		server_crouch(b_crouch_local);
		if (b_crouch_local)
		{
			Crouch();
			b_crouch = true;
		}
		else
		{
			UnCrouch();
			b_crouch = false;
		}
	}
}

void Acharacter_master::server_crouch_Implementation(bool b_crouch_local)
{
	crouch(b_crouch_local);
}

// Called every frame
void Acharacter_master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


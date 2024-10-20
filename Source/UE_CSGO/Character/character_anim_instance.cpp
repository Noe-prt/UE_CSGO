// Fill out your copyright notice in the Description page of Project Settings.


#include "character_anim_instance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void Ucharacter_anim_instance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdateAnimationProperties(DeltaTime);
}

void Ucharacter_anim_instance::UpdateAnimationProperties(float DeltaTime)
{
	APawn*		pawn;
	Acharacter_master*		character;

	pawn = TryGetPawnOwner();
	if (pawn)
	{
		set_movement_speed(pawn->GetVelocity().Size());
		set_movement_direction(CalculateDirection(pawn->GetVelocity(), pawn->GetActorRotation()));
		character = Cast<Acharacter_master>(pawn);
		if (character)
		{
			set_is_in_air(character->GetCharacterMovement()->IsFalling());
			movement_state = character->get_movement_state();
			equipped_weapon_animation_type = character->get_equipped_weapon_animation_type();
			set_aim_direction(character->get_aim_direction());
		}
	}
}


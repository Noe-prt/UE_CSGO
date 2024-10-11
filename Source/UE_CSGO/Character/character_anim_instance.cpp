// Fill out your copyright notice in the Description page of Project Settings.


#include "character_anim_instance.h"

#include "character_master.h"
#include "GameFramework/Character.h"

void Ucharacter_anim_instance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	UpdateAnimationProperties();
}

void Ucharacter_anim_instance::UpdateAnimationProperties()
{
	APawn*		pawn;
	Acharacter_master*		character;

	pawn = TryGetPawnOwner();
	if (pawn)
	{
		movement_speed = pawn->GetVelocity().Size();
		movement_direction = CalculateDirection(pawn->GetVelocity(), pawn->GetActorRotation());
		if (movement_direction < -170.f || movement_direction > 170.f)
		{
			movement_direction = 180.f;
		}
		character = Cast<Acharacter_master>(pawn);
		if (character)
		{
			b_crouch = character->is_crouching();
		}
	}
}


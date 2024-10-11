// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float	movement_speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float	movement_direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool b_crouch;
	void UpdateAnimationProperties();
private:
};

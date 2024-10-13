// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	// Sets default values for this character's properties
	Acharacter_master();
	
protected:
	// Called when the game starts or when spawned
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
	UFUNCTION(Server, Reliable)
	void	server_crouch(bool b_crouch_local);
	UFUNCTION(Server, Reliable)
	void	server_walk(bool b_walk_local);
	UFUNCTION(Server, Reliable)
	void server_add_movement_state(e_movement_state new_state);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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
public:
	USpringArmComponent* get_spring_arm() const {	return spring_arm; };
	UCameraComponent* get_fp_camera() const {	return fp_camera; };
	USkeletalMeshComponent* get_fp_mesh() const {	return fp_mesh; };
	TArray<e_movement_state>& get_movement_state() {  return movement_state; };
};

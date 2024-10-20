// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "UE_CSGO/UE_CSGO.h"
#include "weapon_master.generated.h"

UCLASS()
class UE_CSGO_API Aweapon_master : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aweapon_master();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void on_weapon_sphere_begin_overlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent*		weapon_mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "weapon Properties", meta = (AllowPrivateAccess = "true"))
	USphereComponent*	weapon_sphere;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	e_weapon_animation_type		weapon_animation_type;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	e_weapon_type	weapon_type;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName	weapon_socket;

public:
	FName	get_weapon_socket() const {return weapon_socket;};
	USkeletalMeshComponent*		get_weapon_mesh() const {return weapon_mesh;};
	USphereComponent*	get_weapon_sphere() const {return weapon_sphere;};
	e_weapon_animation_type		get_weapon_animation_type() const {return weapon_animation_type;};
	e_weapon_type		get_weapon_type() const {return weapon_type;};
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "weapon_master.h"

#include "UE_CSGO/Character/character_master.h"
#include "Net/UnrealNetwork.h"

// Sets default values
Aweapon_master::Aweapon_master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	weapon_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("weapon_mesh"));
	weapon_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("weapon_sphere"));

	weapon_mesh->SetupAttachment(RootComponent);
	weapon_mesh->SetRelativeRotation(FRotator(0, -90.f, 0));
	weapon_sphere->SetupAttachment(weapon_mesh);
	weapon_sphere->SetSphereRadius(75.f);
	weapon_sphere->OnComponentBeginOverlap.AddDynamic(this, &Aweapon_master::on_weapon_sphere_begin_overlap);
}

void Aweapon_master::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Aweapon_master, weapon_type);
	DOREPLIFETIME(Aweapon_master, weapon_animation_type);
}

// Called when the game starts or when spawned
void Aweapon_master::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	weapon_mesh->SetSimulatePhysics(true);
	weapon_mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	weapon_mesh->SetCollisionObjectType(ECC_WorldStatic);
	weapon_mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	weapon_mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	weapon_mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	weapon_mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	weapon_mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	weapon_mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	weapon_mesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	weapon_mesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
	
}

void Aweapon_master::on_weapon_sphere_begin_overlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		if (HasAuthority())
		{
			Acharacter_master* character = Cast<Acharacter_master>(OtherActor);
			TArray<Aweapon_master*> weapon_inventory = character->get_weapon_inventory();
			for (int i = 0; i < character->get_weapon_inventory().Num(); i++)
			{
				if (weapon_inventory[i]->weapon_type == weapon_type)
				{
					character->set_can_loot_weapon(false);
					break;
				}
			}
			if (character->get_can_loot_weapon() == true)
			{
				character->loot_weapon(this);
			}
			else
			{
				character->set_can_loot_weapon(true);
			}
		}
	}
}

// Called every frame
void Aweapon_master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


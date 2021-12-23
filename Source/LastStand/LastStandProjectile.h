// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "LastStandProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UDataTable;

USTRUCT(BlueprintType)
struct FCollisionType : public FTableRowBase
{
GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
float Damage;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
bool bProjectileDestroyedAffterApplyEffect;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
int32 AoETargetCap;
};

UCLASS(config=Game)
class ALastStandProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
protected:
	virtual void BeginPlay();

public:
	ALastStandProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data Table", meta = (AllowPrivateAccess = "true")	)
	UDataTable* CollisionDataTable;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageAttributeEffect;

	TMap<FName, int32> MapAoETargetCap;

	TArray<FName> RowNames;
};


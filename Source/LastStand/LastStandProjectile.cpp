// Copyright Epic Games, Inc. All Rights Reserved.

#include "LastStandProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LastStand/LastStandCharacter.h"
#include "AbilitySystemComponent.h"
#include "LSAttributeSet.h"
#include "Components/SphereComponent.h"

/*
Major collision logic is handled in this projectile.
On begin play we'll create a map holding all AoE Target Cap information. 
Problably not the best solution since we're constructing this map every time a projectile is created and is a N size operation
to populate it, there might be a better one, but it's ok for this project.

We bind a dynamic delegate to notify on hit.
OnHit we check the other component CollisionObjectType.
Then, if we have a collision data table we apply the respective CollisionType Effects.
If the other object has an Ability System Component(ASC) we apply an effect on it. Since only LastStandCharacter class
has an ASC we assume that all our Actor with Health(Pawn, Destructibles etc) are derived from it and cast our OtherActor to a LastStandCharacter.
So adding new CollisionObjectTypes is possible but if it's needed to interact with some kind of "Health" it needs to derive from a LastStandCharacter.
If this was a greater project it would be wize to create a more generic "Character" class to bind an ASC. If there is any better implemantation(which probably exists) please let me know.
*/
ALastStandProjectile::ALastStandProjectile()
{
	bReplicates = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ALastStandProjectile::OnHit);		// dynamic delegate to notify when this projectile hit something

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 15.0f;

}

void ALastStandProjectile::BeginPlay()
{
	//AoE Target Cap Map population
	Super::BeginPlay();
	if (CollisionDataTable)
	{
		RowNames = CollisionDataTable->GetRowNames();
		static const FString ContextStringAoE(TEXT("AoETargetCap"));
		for (FName name : RowNames)
		{
			FCollisionType* row = CollisionDataTable->FindRow<FCollisionType>(name, ContextStringAoE);
			if (row)
			{
				MapAoETargetCap.Add(name, 0);
			}
		}
	}
}

void ALastStandProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Check if we're hitting something else
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		FCollisionType* CollisionType;
		//Passing CollisionObjectType(UEnum) to FString to check values in our data table since STRUCT for some reason can't have ENUMs

		const TEnumAsByte<ECollisionChannel> SurfaceEnum = OtherComp->GetCollisionObjectType();
		FString CollisionTypeName = UEnum::GetValueAsString(SurfaceEnum.GetValue());
		
		if (CollisionDataTable)
		{
			static const FString ContextStringCT(TEXT("CollisionType"));
			CollisionType = CollisionDataTable->FindRow<FCollisionType>(FName(*CollisionTypeName), ContextStringCT, true); //Finding row using other comp's CollisionObjectType
			if (CollisionType)
			{
				if (ALastStandCharacter* LSC = Cast<ALastStandCharacter>(OtherActor)) //Cast to ALastStandCharacter since it's the only class we bind a ASC
				{
					UAbilitySystemComponent* ASC = LSC->GetAbilitySystemComponent();
					if (!ASC)
						return;
					float AoECap = *MapAoETargetCap.Find(FName(*CollisionTypeName)); //Check if this projectile hit AoE Target Cap.
					if (AoECap < CollisionType->AoETargetCap)
					{
						MapAoETargetCap.Add(FName(*CollisionTypeName), AoECap + 1); //Add to cap, since TMap don't have dupes we can Add with same key.
						FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext(); //Create Effect Context
						EffectContext.AddSourceObject(this);

						FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageAttributeEffect, 1, EffectContext);

						SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), CollisionType->Damage); //Set Damage tag by SetByCaller using Data table info

						if (SpecHandle.IsValid())
						{
							FActiveGameplayEffectHandle GEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); //Apply Effect
						}

						if (LSC->Attributes->GetHealth() <= 0) //If target life is lower than 0 just destroy it.
							LSC->Destroy();
					}
				}
				if (CollisionType->bProjectileDestroyedAffterApplyEffect) //Destroy projectile if it says on Data Table
					Destroy();
			}
		}
	}
}
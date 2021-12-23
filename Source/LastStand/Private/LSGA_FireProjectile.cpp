// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGA_FireProjectile.h"
#include "LastStand/LastStandCharacter.h"

/*
Gameplay Ability which shoots a projectile.

*/
ULSGA_FireProjectile::ULSGA_FireProjectile()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; //Each ASC only has one instance of the GameplayAbility that is reused between activations.
	AbilityInputID = ELSAbilityInputID::Projectile; //Bind to Right mouse click.
}

void ULSGA_FireProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//Check if player can commit ability.
	//CommitAbility checks for costs and Cooldowns.
	//If we only check on server we guarantee CD verification on server side.
	//(Hey this might not be the best implemantion or it might not even be working as I suppose. Please let me know ;))
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !HasAuthority(&ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (HasAuthority(&ActivationInfo)) //Guarantee only server is activating the ability.
	{

		ALastStandCharacter* Player = Cast<ALastStandCharacter>(GetAvatarActorFromActorInfo());
		if (!Player)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		//Get Player Tranform
		const FRotator SpawnRotation = Player->GetControlRotation();
		const FVector SpawnLocation = ((Player->FP_MuzzleLocation != nullptr) ? Player->FP_MuzzleLocation->GetComponentLocation() : Player->GetActorLocation()) + SpawnRotation.RotateVector(Player->GunOffset);
		FTransform MuzzleTransform = { SpawnRotation, SpawnLocation, {1,1,1} };

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//Spawn projectile
		ALastStandProjectile* Projectile = GetWorld()->SpawnActorDeferred<ALastStandProjectile>(ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->FinishSpawning(MuzzleTransform);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true); //EndAbility
}

bool ULSGA_FireProjectile::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (GEngine) //Fastest way I could find to show Cooldown. Of course it would not be a solution to a shipping project. Would most likely call this in UI.
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player: %s Skill CD: %f"), *ActorInfo->OwnerActor->GetName(), GetCooldownTimeRemaining()));
	}
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	return true;
}

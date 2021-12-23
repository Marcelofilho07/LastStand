// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSGameplayAbility.h"
#include "LastStand/LastStandProjectile.h"
#include "LSGA_FireProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LASTSTAND_API ULSGA_FireProjectile : public ULSGameplayAbility
{
	GENERATED_BODY()
	
public:

	ULSGA_FireProjectile();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ALastStandProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};

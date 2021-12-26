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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* ShootMontage;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(blueprintcallable)
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};

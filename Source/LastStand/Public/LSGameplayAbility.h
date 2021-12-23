// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LastStand/LastStand.h"
#include "LSGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LASTSTAND_API ULSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	ULSGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	ELSAbilityInputID AbilityInputID = ELSAbilityInputID::None;
};

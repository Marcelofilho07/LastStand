// Fill out your copyright notice in the Description page of Project Settings.


#include "LSAttributeSet.h"
#include "Net/UnrealNetwork.h"
/*
Standart Attribute Set implemantation. Only attribute we'll use is Health.
*/
ULSAttributeSet::ULSAttributeSet() {}

void ULSAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULSAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void ULSAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULSAttributeSet, Health, OldHealth);
}

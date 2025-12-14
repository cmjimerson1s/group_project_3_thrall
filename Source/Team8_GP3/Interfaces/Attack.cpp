// Fill out your copyright notice in the Description page of Project Settings.


#include "Attack.h"


// Add default functionality here for any IAttack functions that are not pure virtual.
void IAttack::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack interface was called"));
}

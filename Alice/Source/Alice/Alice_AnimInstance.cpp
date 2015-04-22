// Fill out your copyright notice in the Description page of Project Settings.

#include "Alice.h"
#include "PlayerCharacter.h"
#include "Alice_AnimInstance.h"


UAlice_AnimInstance::UAlice_AnimInstance(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer) 
{
	
}

void UAlice_AnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	owningCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}
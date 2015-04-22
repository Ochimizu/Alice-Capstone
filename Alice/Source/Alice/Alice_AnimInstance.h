// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "Alice_AnimInstance.generated.h"

/**
 * 
 */
class APlayerCharacter;

UCLASS()
class ALICE_API UAlice_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAlice_AnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
		APlayerCharacter* owningCharacter;
};

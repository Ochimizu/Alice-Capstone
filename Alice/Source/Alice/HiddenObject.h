// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HiddenObject.generated.h"

/**
 * 
 */
UCLASS()
class ALICE_API AHiddenObject : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HiddenObject")
		bool wasLocated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HiddenObject")
		bool inRangeOfEcho;

	AHiddenObject(const FObjectInitializer& ObjectInitializer);
};

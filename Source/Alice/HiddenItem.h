// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CollectibleItem.h"
#include "HiddenObject.h"
#include "HiddenItem.generated.h"

/**
 * 
 */
UCLASS()
class ALICE_API AHiddenItem : public ACollectibleItem
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HiddenObject")
		bool wasLocated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HiddenObject")
		bool inRangeOfEcho;

	AHiddenItem(const FObjectInitializer& ObjectInitializer);	
};

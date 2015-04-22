// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "XmlParser.h"
#include "Containers/UnrealString.h"
#include "KeyItemsHandler.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FKeyItemInformation {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItems")
		FString name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItems")
		FString flavourText;

	FKeyItemInformation() {}
};

UCLASS()
class ALICE_API AKeyItemsHandler : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "KeyItems")
		TArray<FKeyItemInformation>	itemInformation;

	AKeyItemsHandler(const FObjectInitializer& ObjectInitializer);
	
	/* Load data for the key items from the file path and apply it the Inventory's key items */
	UFUNCTION(BlueprintCallable, Category = "KeyItems")
		void LoadFile(FString fileName, APlayerCharacter* playerRef);
};

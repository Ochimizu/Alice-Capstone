// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerCharacter.h"
#include "CollectibleItem.generated.h"

/**
 * 
 */
UCLASS()
class ALICE_API ACollectibleItem : public APawn {
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
		APlayerCharacter* player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
		bool isInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
		USphereComponent* trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
		TEnumAsByte<EItemType> type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
		FString name;

	/* Constructor */
	ACollectibleItem(const FObjectInitializer& ObjectInitializer);

	/* Sets the Type of object and the corresponding name */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetType(EItemType t);

	/* Handles when the player enters the trigger radius */
	UFUNCTION()
		void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/* Handles when the player leaves the trigger radius */
	UFUNCTION()
		void OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* Adds the item to the player's inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void PickedUp();

	/* If the player is within range at the text to the screen */
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
		void DisplayTextWidget();

	/* If the player leaves the range hide the text widget */
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
		void HideTextWidget();
	
};
